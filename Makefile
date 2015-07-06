all: clean build

build: 
	platformio run
##	 ano build -m pro --cpu 16MHzatmega328

upload:
	platformio run -t upload --upload-port /dev/ttyUSB$(tty)

clean:
	platformio run -t clean

serial:
	platformio serialports monitor -p  /dev/ttyUSB$(tty)

