
add_executable(test.component.inserters.real
	test.component.inserters.real.cpp
)

target_link_Pantheios_core(test.component.inserters.real)

target_link_libraries(test.component.inserters.real
	PRIVATE
		Pantheios.be.test
		Pantheios.bec.test
)

target_link_libraries(test.component.inserters.real
	PRIVATE
		Pantheios.fe.all
)

target_link_Pantheios_util(test.component.inserters.real)

target_link_xTests(test.component.inserters.real)

define_target_compile_options(test.component.inserters.real)


# ############################## end of file ############################# #

