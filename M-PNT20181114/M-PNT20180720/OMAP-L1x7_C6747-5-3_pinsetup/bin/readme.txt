Here are some basic instructions to install and use pin-mux tool. 


- Run c:\pinsetup\bin\PinSetup.exe 

- This only contains the pins where there are decisions to be made on pinmuxing. 
  Some functional pins are not listed here (e.g. JTAG, USB, RTC, ...)

- Device buttons will give you some ideas on pins that drop as you change part numbers.

- If you click the different Device Buttons (i.e.: DA805) they will gray out all 
  the pins not available on the device and show you what is available.

- Don't spend too much time on the Configuration Buttons. 
  We haven't spent a lot of time defining what pins are set for each configuration.
  So, we currently just set some random pins.

- If you click the different Peripherals Buttons, related pins will be selected.

- You can play around with the Edit -> Find. For example, use it to check (i.e. select)
  pins and you can see how it handles collisions.

- Double clicking on a cell enables the pin (or disables it if it's enabled).

- You can copy the registers to the clipboard and paste on desired file.
  Or, you can save them to a .h file from File -> Save -> Header File.

- You can save your pin configurations from File -> Save -> Pin Selections
  Also, you can load your pin configurations from File -> Load -> Pin File.

This tool is in basic form and may not be perfect.
Please let us know if you find problems or have questions.

