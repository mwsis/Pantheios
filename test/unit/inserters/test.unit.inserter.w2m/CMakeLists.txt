
add_executable(test.unit.inserter.w2m
	test.unit.inserter.w2m.cpp
)

target_link_Pantheios_core(test.unit.inserter.w2m)

target_link_libraries(test.unit.inserter.w2m
	PRIVATE
		Pantheios.be.null
		Pantheios.bec.null
)

target_link_libraries(test.unit.inserter.w2m
	PRIVATE
		Pantheios.fe.null
)

target_link_Pantheios_util(test.unit.inserter.w2m)

target_link_xTests(test.unit.inserter.w2m)

define_target_compile_options(test.unit.inserter.w2m)


# ############################## end of file ############################# #

