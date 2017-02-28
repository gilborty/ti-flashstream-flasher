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
        # Usage: self.dispatch[char](args)
        self.dispatch = {
            'C': self._handle_compare_command,
            ';': self._handle_comment,
            'R': self._handle_read_command,
            'W': self._handle_write_command,
            'X': self._handle_wait_command
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

    def _handle_comment(self, line):
        """Handle the ; (Comment)

        Example:
            ;This is a comment
        """
        print "Comment {}".format(line)
        pass
    def _handle_compare_command(self, line):
        """Handles the C (compare): command

        Example:
            C: I2CAddr RegAddr Byte0 Byte1 Byte2 ...

            Read from the register, and compare the result with the list of bytes
        """
        print 'Comparing'
        # Get the i2c address, register, and number of bytes
        i2c_addr, reg_addr, data = self._parse_payload(line)

        # Read the value
        read_value = self.bus.read_block_data(i2c_addr, reg_addr)

        # make sure there is data 
        if len(read_value) > 0:
            # Compare
            for idx, value in enumerate(data):
                print(read_value[idx], value)
        else:
            print 'No values to compare. read_value: {} data: {}'.format(read_value, data)


    def _handle_read_command(self, line):
        """Handles the R (read): command

        Example:
            R: I2CAddr RegAddr NumBytes
        """
        # Get the i2c address, register, and number of bytes
        i2c_addr, reg_addr, data = self._parse_payload(line)
        result = self.bus.read_block_data(i2c_addr, reg_addr)

    def _handle_wait_command(self, line):
        """Handles the X (wait): command

        Example:
            X: WaitInMS
        """
        # Convert to seconds for time.sleep()        
        seconds = float(line) / 1000.0

        print 'Waiting for {} seconds'.format(seconds)
        time.sleep(seconds)

        print 'Done waiting.'

    def _handle_write_command(self, line):
        """Handles the W (write): command

        Example:
            W: I2CAddr RegAddr Byte0 Byte1 Byte2 ...

        """
        #print "Handling write command: {}".format(line)
        i2c_addr, reg_addr, data = self._parse_payload(line)
        print 'Writing to register: {} on i2c device: {}'.format(hex(reg_addr), hex(i2c_addr))
        
        # Actually write the data
        self._write_data(i2c_addr, reg_addr, data)

    def _write_data(self, i2c_addr, reg_addr, data):
        """Writes data over i2c using the SMBus object

        Args:
            i2c_addr (hex): The i2c device to use
            reg_addr (hex): The register to write to 
            data (list): The data to write

        Returns:

        """
        # Write to the address 
        # SMBus is limited to 32 bytes. So, split the data
        print 'Writing: {}'.format(data)
        if len(data) > 32:
            print 'Transaction is larger than SMBus can handle. Sending in chunks'
            
        print 'Wrote {}'.format(data)
        self.bus.write_i2c_block_data(i2c_addr, reg_addr, data)
        

        
    def _parse_payload(self, payload):
        """ Parse a TI FileStream line into constituent bits

        Typically, the FileStream payload looks like this:
        [I2CAddr] [RegAddr] [Stuff (n length)]

        Our goal is to split it into this stuff

        Args:
            payload (string): The payload to be parsed
        Returns:
            i2c_addr, reg_addr, [stuff] (tuple): The parsed payload
        """
        # Split by spaces
        data_list = payload.split(' ')
        
        # i2c_addr is index 0 and is bit shifted right by 1
        i2c_addr = data_list[0]
        i2c_addr = int(i2c_addr, 16) >> 1

        # Reg addr is index 1
        reg_addr = int(data_list[1], 16)

        # Data is everything else
        if len(data_list) > 2:
            data = [int(x, 16) for x in data_list[2:]]
        else:
            data = None

        return i2c_addr, reg_addr, data
