#!/usr/env/python
# -*- coding: utf-8 -*-
"""ti-flashstream-flasher: A firmware flasher for the TI FlashStream file format

Reference documentation: http://www.ti.com/lit/an/slua541a/slua541a.pdf

Example:
	$ python main.py [-f | --flash] [path to file.bqfs | file.ddfs]

Todo:

"""
import argparse
import os
import smbus
import sys
import time

# Our own modules
import flashstream

def create_i2c_bus_handle(port=1):
    """Creates an SMBus handle to the specified i2c port

    Args:
        port (int): The i2c port to interface with Ex: port=1 is /dev/i2c-1
    
    Returns:
        bus (SMBus): An SMBus object to the i2c device
    """
    try:
        bus = smbus.SMBus(port)
    except IOError:
        # Port could'nt be opened
        i2c_port = '/dev/i2c-' + str(port)
        print('Could not open: {}. Aborting'.format(i2c_port))
        sys.exit(1)
    return bus

def is_valid_image_file(file_in):
    """Checks the user provided path to the firmware image to see if it is valid

    Args:
        file_in (str): The path to the image file

    Returns:
        bool: if the image is valid. True for success, False otherwise
    """
    
    valid_extensions = [
        '.bqfs',
        '.dffs',
        '.fs'
    ]

    # Check if the file is, ya know, a file
    if not os.path.isfile(file_in):
        print("{} is not a file".format(file_in))
        return False

    # Check if it has the correct extension
    filename, file_extension = os.path.splitext(file_in)

    for extension in valid_extensions:
        if file_extension == extension:
            return True
    
    return False

def parse_image(image):
    """Parses a TI FlashStream file

    Args:
        image (str): The path to the image file
    Returns:
        flashstream_list (list): The parsed contents of the file, in a list
    """
    # Open the file
    with open(image) as f:
        data = f.readlines()
    
    # Clean, in this case strip \r \n
    data = [x.strip() for x in data]

    return data

def main():
    """Main function, the main entry point to the script
    
    Args:
        None
    Returns:
        None
    """

    # Get the argurments passed from the command line
    argument_parser = argparse.ArgumentParser(description='Firmware flasher for TI Fuel Gauge')
    argument_parser.add_argument('-f', '--flash', help='The path to the firmware.srec file', required=True)
    argument_parser.add_argument('-p', '--port', help='The i2c port to use. Default: 1', type=int, default=1)
    arguments = vars(argument_parser.parse_args())
    
    # Make sure this is actually a file, has the correct extension, etc.
    image_file = arguments['flash']
    if arguments['port']:
        port = arguments['port']

    print('Using: {} to flash fuel gauge'.format(image_file))

    if is_valid_image_file(image_file):
        print('{} is valid, starting flashing process.'.format(image_file))
        
        # Parse the image file for the flashstream class
        flashstream_list = parse_image(image_file)

        # Create an i2c bus object
        bus = create_i2c_bus_handle(port)
        # Give that to the FileStream class for parsing
        # But first create that object
        filestream_handler = flashstream.FlashStream(bus, flashstream_list)

        # Try to flash the image
        if filestream_handler.flash():
            print('Flashing was successful, exiting.')
            sys.exit(0)
        else:
            print('Something went wrong flashing the device. Exiting')
            sys.exit(1)

    else:
        print('{} is not a valid image file. Aborting flashing process.'.format(image_file))
        sys.exit(1)


if __name__ == "__main__":
    main()