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
| 5 | Inline an `fe.simple` ceiling cache | ❌ | Rejected. It put one frontend's ceiling into the core. |
| 6 | Narrow the `try` in `pantheios_log_n()` | ⚠️ | Applied. No measurable change (`short` 31.0, `wide` 178.6, `oversize` 81.1). |
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

Rejected. Caching **fe.simple**'s ceiling in the core, or teaching `pantheios_isSeverityLogged()` about that frontend, makes the core depend on one frontend. `pantheios_isSeverityLogged()` stays a call to `pantheios_fe_isSeverityLogged()`. `filtered` and `severity` remain 3.4 and 2.4 ns/call.


### 6. Narrow the `try` in `pantheios_log_n()`

Applied. The buffer is constructed empty, and `resize(1 + n)` is the only call inside the `try`. The slice copy and `pantheios_dispatch()` sit after it. An exception from the copy now propagates; previously `std::exception` there became a bail-out and `PANTHEIOS_INIT_RC_UNSPECIFIED_EXCEPTION`. Allocation failure is unchanged: `resize()` returning false, or `std::bad_alloc`, still bails out with `PANTHEIOS_INIT_RC_OUT_OF_MEMORY`.

`short`, `wide`, and `oversize` were the rows that might move. Against the post-item-4 run they are 31.0, 178.6, and 81.1 ns/call (previously 30.8, 174.8, and 83.0). That is noise. The behaviour change remains: an exception from the copy propagates, where it used to become a bail-out. Worth reverting unless that propagation is wanted for its own sake.


### 7. Internal buffer for small inserters

`pantheios_inserterAllocate()` is `malloc`, and `pantheios_inserterDeallocate()` is `free`. Integer, pointer, and similar inserters format a bounded string and can use an internal array, allocating only when the formatted result exceeds it.

This is outside `pantheios_log_n()` and `pantheios_logvprintf()`. **test.performance.core.log_dispatch** does not call an inserter. The existing **test.performance.inserters.w2m** and **test.performance.inserters.m2w** programs time conversion, not this allocate/format/free cycle.



<!-- ########################### end of file ########################### -->
