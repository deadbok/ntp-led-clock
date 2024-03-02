#!/usr/bin/env python3
"""
Convert time zone data in json format to something usable in a html <select>
:copyright: (c) 2022 by Martin Grønholdt.
"""
import json

def main():
    """
    Main code.
    """
    f = open('zones.json')
  
    tz_data = json.load(f)

    for region, tzd in tz_data.items():
        print("<option value=\"" + tzd + "\">" + region + "</option>") 
  
    f.close()


if __name__ == "__main__":
    main()