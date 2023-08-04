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
	docker build -f docker/server.Dockerfile . -t tg_weather_bot:latest
run:
	docker run --env-file=.env --name tg_weather_bot -d tg_weather_bot:latest
run2:
	docker run --env-file=.env --name tg_weather_bot tg_weather_bot:latest 
stop:
	docker stop tg_weather_bot || true
	docker rm tg_weather_bot || true
restart: build-server stop run


rebuild-run: stop build-server run
rebuild-run2: stop build-server run2

debug: build-debugger run-debugger

build-debugger:
	docker build -f docker/gdb.Dockerfile . -t c_debugger:latest

run-debugger:
	docker stop tg_weather_bot_debugger || true
	docker rm tg_weather_bot_debugger || true
	docker run -it --env-file=debug.env --name tg_weather_bot_debugger c_debugger:latest
