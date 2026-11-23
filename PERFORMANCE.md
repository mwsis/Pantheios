# Pantheios - Performance <!-- omit in toc -->


## Table of Contents <!-- omit in toc -->

- [Core](#core)
  - [1. Concatenate slices with `memcpy`](#1-concatenate-slices-with-memcpy)
  - [2. Replace `auto_buffer` with a local array](#2-replace-auto_buffer-with-a-local-array)
  - [3. Pass the slice array to the back-end](#3-pass-the-slice-array-to-the-back-end)
  - [4. Stop zero-initialising the `printf` stack buffer](#4-stop-zero-initialising-the-printf-stack-buffer)
  - [5. Inline an `fe.simple` ceiling](#5-inline-an-fesimple-ceiling)
  - [6. Narrow the `try` in `pantheios_log_n()`](#6-narrow-the-try-in-pantheios_log_n)
  - [7. Internal buffer for small inserters](#7-internal-buffer-for-small-inserters)


## Core

**Pantheios** core timings come from **test.performance.core.log_dispatch**, linked to **fe.simple** and **be.null**. Each row is the minimum of 3 samples after 2 warm-ups. Light cases run 1,000,000 iterations; `oversize` and `printf-long` run 100,000. The Release baseline (23rd November 2026) was:

| case | ns/call |
| --- | --- |
| `filtered` | 3.4 |
| `severity` | 2.4 |
| `short` | 31.3 |
| `wide` | 174.8 |
| `oversize` | 80.3 |
| `appl` | 41.8 |
| `printf` | 149.6 |
| `printf-long` | 387.3 |


| # | Suggestion | Status | Comment |
| --- | --- | --- | --- |
| 1 | `memcpy` per slice in `pantheios_log_n()` | | Unmeasured. |
| 2 | Hand-written stack buffer instead of `auto_buffer` | ❌ | Withdrawn. |
| 3 | Slice-array back-end entry (`writev`) | | Unmeasured. API addition. |
| 4 | Stop zero-initialising the `printf` stack buffer | ✅ | 149.6 → 75.3 ns/call. `printf-long` 387.3 → 332.3. |
| 5 | Inline an `fe.simple` ceiling cache | | Unmeasured. Gain looks small. |
| 6 | Narrow the `try` in `pantheios_log_n()` | | Unmeasured. Only if a profile shows it. |
| 7 | Internal buffer for small inserters | | Unmeasured. Not covered by this program. |


### 1. Concatenate slices with `memcpy`

`pantheios_log_n()` in **src/core/api.cpp** sums slice lengths with `std::accumulate` and `member_selector`, then copies them with `std::copy` into `cstring_concatenator`. Both passes walk the slice array through iterator adaptors.

Replace that with one loop: a running pointer into the `auto_buffer`, and `memcpy` of each slice. `pantheios_logassertfail()` already copies its two parts that way. Keep the NUL after the last byte, and keep the length check that the written count equals the sum.

`short` (4 slices) and `wide` (32 slices) are the rows that should move. `oversize` still pays for the heap spill once the statement exceeds the 2048-byte internal buffer.


### 2. Replace `auto_buffer` with a local array

Withdrawn. The suggestion was a local `pantheios_char_t buf[2048]` plus a hand-written heap spill when `n + 1` exceeds that. `pantheios::util::auto_buffer_selector` already selects `stlsoft::auto_buffer` with an internal size of 2048, which is that stack array and that spill. The selector is compile-time only. A statement that fits never calls the allocator. Replacing it would duplicate `auto_buffer` and drop the allocation-failure handling it already provides.


### 3. Pass the slice array to the back-end

`pantheios_dispatch()` calls `pantheios_be_logEntry()` with one contiguous string, so the core always concatenates before the back-end copies again into a file, a socket, or syslog.

Add a back-end entry that receives the severity and the slice array. A file or socket back-end can then `writev` (or the platform equivalent) without the core materialising one buffer. Leave `pantheios_be_logEntry()` unchanged. `be.null` returns before it looks at the bytes, so this program will not show the change. It needs a back-end that writes.


### 4. Stop zero-initialising the `printf` stack buffer

Applied in `pantheios_logvprintf()`. The stack buffer is `PANTHEIOS_CORE_LOGPRINTF_STACK_SIZE` (4096 by default). It was value-initialised with `= { '\0' }`, which clears every element on each emitted statement. The negative-length fallback only needs a leading NUL before `vsnprintf`, and the truncation path only needs the trailing NUL written after it.

`printf` went from 149.6 ns/call to 75.3. `printf-long` went from 387.3 to 332.3: the same clearing cost, on top of formatting a 5000-character string that still truncates to the stack buffer. The other rows stayed within a few nanoseconds of the baseline. The `printf` and `printf-long` cases call `pantheios_logprintf()`, which builds a `va_list` and calls `pantheios_logvprintf()`.


### 5. Inline an `fe.simple` ceiling

Every C++ `log()` calls `pantheios_isSeverityLogged()` before it builds slices. That is one out-of-line call into the frontend. For **fe.simple** the body is `(severity & 0x0f) <= pantheios_fe_severity_ceiling_`.

Cache that ceiling in the core, update it from `pantheios_fe_simple_setSeverityCeiling()`, and inline the compare for this frontend. Frontends that decide per back-end (**fe.N**, the Windows registry frontend) keep the real call. `be.N` would still call the frontend once per back-end.

`filtered` and `severity` are already about 3.4 and 2.4 ns/call, so the gain is a call overhead on statements that do no other work.


### 6. Narrow the `try` in `pantheios_log_n()`

The sum, the `auto_buffer`, the copy, and `pantheios_dispatch()` sit in one `try` that catches `std::bad_alloc` and `std::exception`. On compilers that emit unwind tables for the whole function, that frame is on the success path.

Move the catch so it covers the allocation only, and leave the copy and the dispatch outside it. Do this only if a profile shows those tables in the hot samples. Contract checks stay where they are.


### 7. Internal buffer for small inserters

`pantheios_inserterAllocate()` is `malloc`, and `pantheios_inserterDeallocate()` is `free`. Integer, pointer, and similar inserters format a bounded string and can use an internal array, allocating only when the formatted result exceeds it.

This is outside `pantheios_log_n()` and `pantheios_logvprintf()`. **test.performance.core.log_dispatch** does not call an inserter. The existing **test.performance.inserters.w2m** and **test.performance.inserters.m2w** programs time conversion, not this allocate/format/free cycle.



<!-- ########################### end of file ########################### -->
