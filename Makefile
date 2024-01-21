TAG?=latest
ENV_TAG=$(SOURCE_TAG)
DOCKER_REPOSITORY?=

ifneq ($(ENV_TAG),) 
    TAG=$(ENV_TAG)
endif

default: help

.PHONY: help
help: # Show help for each of the Makefile recipes.
	@grep -E '^[a-zA-Z0-9 -]+:.*#'  Makefile | sort | while read -r l; do printf "\033[1;32m$$(echo $$l | cut -f 1 -d':')\033[00m:$$(echo $$l | cut -f 2- -d'#')\n"; done
.PHONY: build-builder
build-builder: # Build docker-image with gcc and C libs used to build application
	docker build -f docker/build.Dockerfile . -t c_compiler:latest
.PHONY: build
build: build-builder # Build application source code
	mkdir -p ./build
	docker run -v $(PWD)/src:/app c_compiler:latest
	rm build/weather-bot -f
	mv src/weather-bot build/weather-bot
	cd src && make clean
.PHONY: clean
clean: # Clean build artifacts
	cd src && make clean
	rm build -rf
.PHONY: build-server
build-server: build # Build application docker-image
	mkdir -p data
	docker build -f docker/server.Dockerfile . -t tg_weather_bot:$(TAG)
.PHONY: run
run: # Run containerized app
	docker run --env-file=.env -v $(PWD)/data:/app --name tg_weather_bot -d tg_weather_bot:latest
.PHONY: run2
run2: # Run containerized app with console output
	docker run --env-file=.env -v $(PWD)/data:/app --name tg_weather_bot tg_weather_bot:latest 
.PHONY: stop
stop: # Stop containerized app
	docker stop tg_weather_bot || true
	docker rm tg_weather_bot || true
.PHONY: restart
restart: build-server stop run # Rebuild and restart containerized app
.PHONY: publish
publish: build-server # Publish containerized app on dockerhub
	docker tag tg_weather_bot:$(TAG) $(DOCKER_REPOSITORY):$(TAG)
	docker push $(DOCKER_REPOSITORY):$(TAG)
	docker tag tg_weather_bot:$(TAG) $(DOCKER_REPOSITORY):latest
	docker push $(DOCKER_REPOSITORY):latest
.PHONY: rebuild-run
rebuild-run: stop build-server run # Stop and rebuild containerized app
.PHONY: rebuild-run2
rebuild-run2: stop build-server run2 # Stop and rebuild containerized app, run with console output
.PHONY: debug
debug: build-debugger run-debugger # Run with debugger GDB container
.PHONY: profile
profile: build-profiler run-profiler # Run profiling contariner with Gprof

valgrind: build-valgrind run-valgrind # Run profiling contariner with valgrind

build-debugger: # Run debugger container with GDB
	docker build -f docker/gdb.Dockerfile . -t c_debugger:latest

build-profiler:  # Run debugger container with Gprof
	docker build -f docker/gprof.Dockerfile . -t c_profiler:latest

build-valgrind: # Build profiling contariner with valgrind
	docker build -f docker/valgrind.Dockerfile . -t c_valgrind:latest	

run-debugger: # Run debugger without rebuild
	docker stop tg_weather_bot_debugger || true
	docker rm tg_weather_bot_debugger || true
	docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it --env-file=debug.env --name tg_weather_bot_debugger c_debugger:latest

run-profiler: # Run profiler without rebuild
	mkdir -p profiler
	docker stop tg_weather_bot_profiler || true
	docker rm tg_weather_bot_profiler || true
	docker run --cap-add=SYS_PTRACE -it --security-opt seccomp=unconfined -v $(PWD)/profiler:/app/data --env-file=debug.env --name tg_weather_bot_profiler c_profiler:latest

run-valgrind: # Run valgirnd profiler without rebuild
	mkdir -p valgrind
	docker stop tg_weather_bot_valgrind || true
	docker rm tg_weather_bot_valgrind || true
	docker run --ulimit nofile=262144:262144 --cap-add=SYS_PTRACE -it --security-opt seccomp=unconfined -v $(PWD)/valgrind:/app/data --env-file=debug.env --name tg_weather_bot_valgrind c_valgrind:latest