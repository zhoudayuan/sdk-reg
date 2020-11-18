.PHONY: clean All

All:
	@echo "----------Building project:[ wujian100_open-hello_world - BuildSet ]----------"
	@
	@make -f "wujian100_open-hello_world.mk" MakeIntermediateDirs && make -f "wujian100_open-hello_world.mk" -j 4
	@echo Executing Post Build commands ...
	@export CDKPath="D:/C-Sky/CDK" ProjectPath="D:/prj/wujian100/PRJ-RYAM001A/code/zhong/sdk-reg/projects/examples/hello_world/CDK" && "D:/prj/wujian100/PRJ-RYAM001A/code/zhong/sdk-reg/projects/examples/hello_world/CDK//../../../../utilities//aft_build.sh"
	@echo Done

clean:
	@echo "----------Cleaning project:[ wujian100_open-hello_world - BuildSet ]----------"
	@make -f  "wujian100_open-hello_world.mk" clean
