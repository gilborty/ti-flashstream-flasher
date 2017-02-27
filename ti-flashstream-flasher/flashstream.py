#!/usr/env/python
# -*- coding: utf-8 -*-
"""flashstream: A module for interfacing with the TI FlashStream file format

Reference documentation: http://www.ti.com/lit/an/slua541a/slua541a.pdf

Example:

Todo:

"""

class FlashStream(object):
    """Interface to a FlashStream list
    
    """

    def __init__(self, bus, flashstream_list):
        """ Init
        
        Takes a list which is a parsed, FlashStream file. Also, a handle to the SMBus object to interface with the i2c device
        
        Args:
            bus (SMBus): A handle to the i2c interface
            flashstream_list (list): A parsed, FlashStream file
        """
        self.bus = bus
        self.flashstream_list = flashstream_list
    
    def flash(self):
        """ Begins the flashing process

        Args:
            None
        Returns:
            boolean: True if successful, False otherwise
        """
        pass
        
