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

        # Setup the dispatch dict
        # Usage:
        # self.dispatch[char](args)
        self.dispatch = {
            'C': self._handleComapareCommand,
            'R': self._handleReadCommand,
            'W': self._handleWriteCommand,
            'X': self._handleWaitCommand
        }

        
    
    def flash(self):
        """ Begins the flashing process

        Args:
            None
        Returns:
            boolean: True if successful, False otherwise
        """
        # Iterate through the list


        
        pass

    def _handleComment(self, line):
        """Handle the ; (Comment)
        """
        pass
    def _handleComapareCommand(self, line):
        """Handles the C (compare): command
        """
        pass
    def _handleReadCommand(self, line):
        """Handles the R (read): command
        """
        pass
    def _handleWaitCommand(self, line):
        """Handles the X (wait): command
        """
        pass
    def _handleWriteCommand(self, line):
        """Handles the W (write): command
        """
        pass
        
