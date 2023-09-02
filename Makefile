TAG?=latest
ENV_TAG=$(SOURCE_TAG)
DOCKER_REPOSITORY?=

ifneq ($(ENV_TAG),) 
    TAG=$(ENV_TAG)
endif


build-builder:
	docker build -f docker/build.Dockerfile . -t c_compiler:latest
build: build-builder
	mkdir -p ./build
	docker run -v $(PWD)/src:/app c_compiler:latest
	rm build/weather-bot -f
	mv src/weather-bot build/weather-bot
	cd src && make clean
clean:
	cd src && make clean
	rm build -rf
build-server: build
	mkdir -p data
	docker build -f docker/server.Dockerfile . -t tg_weather_bot:$(TAG)
run:
	docker run --env-file=.env -v $(PWD)/data:/app --name tg_weather_bot -d tg_weather_bot:latest
run2:
	docker run --env-file=.env -v $(PWD)/data:/app --name tg_weather_bot tg_weather_bot:latest 
stop:
	docker stop tg_weather_bot || true
	docker rm tg_weather_bot || true
restart: build-server stop run

publish: build-server
	docker tag tg_weather_bot:$(TAG) $(DOCKER_REPOSITORY):$(TAG)
	docker push $(DOCKER_REPOSITORY):$(TAG)
	docker tag tg_weather_bot:$(TAG) $(DOCKER_REPOSITORY):latest
	docker push $(DOCKER_REPOSITORY):latest

rebuild-run: stop build-server run
rebuild-run2: stop build-server run2

debug: build-debugger run-debugger

profile: build-profiler run-profiler

build-debugger:
	docker build -f docker/gdb.Dockerfile . -t c_debugger:latest

build-profiler:
	docker build -f docker/gprof.Dockerfile . -t c_profiler:latest	

run-debugger:
	docker stop tg_weather_bot_debugger || true
	docker rm tg_weather_bot_debugger || true
	docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it --env-file=debug.env --name tg_weather_bot_debugger c_debugger:latest

run-profiler:
	mkdir -p profiler
	docker stop tg_weather_bot_profiler || true
	docker rm tg_weather_bot_profiler || true
	docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it -v $(PWD)/profiler:/app/data --env-file=debug.env --name tg_weather_bot_profiler c_profiler:latest
