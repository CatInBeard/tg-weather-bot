build-builder:
	 docker build -f docker/build.Dockerfile . -t c_compiler:latest
build: build-builder
	mkdir -p ./build
	docker run -v ./src:/app c_compiler:latest
	rm build/weather-bot -f
	mv src/weather-bot build/weather-bot
	cd src && make clean
clean:
	cd src && make clean
	rm build -rf
build-server: build
	docker build -f docker/server.Dockerfile . -t tg_weather_bot:latest
run:
	docker run --name tg_weather_bot -d tg_weather_bot:latest
run2:
	docker run --name tg_weather_bot tg_weather_bot:latest 
stop:
	docker stop tg_weather_bot
	docker rm tg_weather_bot
restart: build-server stop run
