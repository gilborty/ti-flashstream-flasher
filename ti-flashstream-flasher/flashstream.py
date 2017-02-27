#!/usr/env/python
# -*- coding: utf-8 -*-
"""flashstream: A module for interfacing with the TI FlashStream file format

Reference documentation: http://www.ti.com/lit/an/slua541a/slua541a.pdf

Example:

Todo:

"""
import time

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
            ';': self._handleComment,
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
        # Iterate through the list and handle each line
        for data_line in self.flashstream_list:
            # Get the command character 
            command = self._get_command_char(data_line)

            # Clean up the payload
            payload = self._clean_payload(data_line)

            # Deal with it
            self.dispatch[command](payload)
        
        return True

    def _clean_payload(self, data):
        """Parses a data line to get the payload we want

        Args:
            data (str): The line to be parsed
        Returns:
            payload (string): The payload to be dispatched
        """
        payload = data.split(':')

        # Handle comments correctly
        if len(payload) > 1:
            payload = payload[1].strip()
        
        return payload

    def _get_command_char(self, data):
        """Parses a line to get the first command character
        
        Data lines look like this:
        [command char]: [data string]

        Args:
            data (str): The line to be parsed
        Returns:
            command (char): The parsed out command character
        """
        command_char = data[:1]
        return command_char


    def _handleComment(self, line):
        """Handle the ; (Comment)
        """
        print "Comment {}".format(line)
        pass
    def _handleComapareCommand(self, line):
        """Handles the C (compare): command
        """
        #print "Handling compare command: {}".format(line)
        pass
    def _handleReadCommand(self, line):
        """Handles the R (read): command
        """
        #print "Handling read command: {}".format(line)
        pass
    def _handleWaitCommand(self, line):
        """Handles the X (wait): command
        """
        print 'Waiting for {} milliseconds'.format(line)
        
        milliseconds = float(line) / 1000.0
        time.sleep(milliseconds)

        print 'Done waiting. '

    def _handleWriteCommand(self, line):
        """Handles the W (write): command
        """
        #print "Handling write command: {}".format(line)
        pass
        
