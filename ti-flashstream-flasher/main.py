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

def is_valid_image_file(file_in):
    """Checks the user provided path to the firmware image to see if it is valid

    Args:
        file_in (str): The path to the image file

    Returns:
        bool: if the image is valid. True for success, False otherwise
    """
    
    valid_extensions = [
        '.bqfs',
        '.bq.fs',
        '.dffs',
        '.df.fs'
    ]


    # Check if the file is, ya know, a file
    if not os.path.isfile(file_in):
        print("{} is not a file".format(file_in))
        return False

    # Check if it has the correct extension
    filename, file_extension = os.path.splitext(file_in)

    for extension in valid_extensions:
        print (extension, file_extension)
        if not file_extension == extension:
            return False
    
    return True


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
        #flash_image(image_file, port)
    else:
        print('{} is not a valid image file. Aborting flashing process.'.format(image_file))
        sys.exit()


if __name__ == "__main__":
    main()