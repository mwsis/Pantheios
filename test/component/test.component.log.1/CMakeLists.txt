
add_executable(test.component.log.1
	test.component.log.1.cpp
)

target_link_Pantheios_core(test.component.log.1)

target_link_libraries(test.component.log.1
	PRIVATE
		Pantheios.be.test
		Pantheios.bec.test
)

target_link_libraries(test.component.log.1
	PRIVATE
		Pantheios.fe.all
)

target_link_Pantheios_util(test.component.log.1)

target_link_xTests(test.component.log.1)

define_target_compile_options(test.component.log.1)


# ############################## end of file ############################# #

