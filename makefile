#http://ed.am/posts/using-an-external-editor-for-arduino-development



BOARD = mega2560
# uno
# OBJECTS += BMP085/BMP085.o
include ./arduino.mk 

	# @echo BLAKE $(LIBRARIES)
	# @echo test $(notdir $(wildcard $(ARDUINODIR)/libraries/*))
	# @echo ardlibs $(ARDUINOLIB) 
	# @echo objs $(OBJECTS)

#To compile type `$ make`

#To upload to arduino: `$ make upload`
flash: upload


#To read serial: `$ make monitor`
# To stop: ctrl+a, k

setups:
	cp ./utils/arduino.mk /usr/local/lib/avr/arduino.mk 

## Notes
# [1] /usr/share/arduino/libraries/
# /usr/share/arduino/libraries/Wire

## Morals
# moral of the story is just put the library in the [1] above
