.PHONY: clean All

All:
	@echo "----------Building project:[ wujian100_open-hello_world - BuildSet ]----------"
	@cd "D:\wujian\fpga-bin\prj_hello\sdk\projects\examples\hello_world\CDK" && make -f "wujian100_open-hello_world.mk" clean_internal 
	@
	@cd "D:\wujian\fpga-bin\prj_hello\sdk\projects\examples\hello_world\CDK" && make -f "wujian100_open-hello_world.mk" MakeIntermediateDirs && make -f "wujian100_open-hello_world.mk" -j 4
	@echo Executing Post Build commands ...
	@cd "D:\wujian\fpga-bin\prj_hello\sdk\projects\examples\hello_world\CDK" && export CDKPath="D:/C-Sky/CDK" ProjectPath="D:/wujian/fpga-bin/prj_hello/sdk/projects/examples/hello_world/CDK" && "D:/wujian/fpga-bin/prj_hello/sdk/projects/examples/hello_world/CDK//../../../../utilities//aft_build.sh"
	@echo Done

clean:
	@echo "----------Cleaning project:[ wujian100_open-hello_world - BuildSet ]----------"
	@cd "D:\wujian\fpga-bin\prj_hello\sdk\projects\examples\hello_world\CDK" && make -f  "wujian100_open-hello_world.mk" clean
