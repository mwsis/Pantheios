
add_executable(test.component.inserters.processid
	test.component.inserters.processid.cpp
)

target_link_Pantheios_core(test.component.inserters.processid)

target_link_libraries(test.component.inserters.processid
	PRIVATE
		Pantheios.be.test
		Pantheios.bec.test
)

target_link_libraries(test.component.inserters.processid
	PRIVATE
		Pantheios.fe.all
)

target_link_Pantheios_util(test.component.inserters.processid)

target_link_xTests(test.component.inserters.processid)

define_target_compile_options(test.component.inserters.processid)


# ############################## end of file ############################# #

