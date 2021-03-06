# BruinNav

C++ program that builds navigation system that loads and indexes OpenStreetMap geospatial data, and implements A* search algorithm to output optimal turn by turn directions from user-input source to destination. mapData.txt stores the OpenStreetMap data of UCLA, Westwood, West Los Angeles, West Hollywood, Brentwood, and Santa Monica. 


After the user inputs valid attraction as the starting point and a valid attraction as the destination, the program outputs a cardinal direction along with a distance with a precision of .001 miles to proceed walking or a directional turn if changing streets.

EXAMPLE: Directions from UCLA dorm building Rieber Hall to popular ice cream store in Westwood, Diddy Riese:


Detailed                                                               |  Less Detailed
:---------------------------------------------------------------------:|:-------------------------:
![example1-image](https://i.gyazo.com/df69a9a37833be70f395a59204c0a2c4.png)  |  ![example2-image](https://i.gyazo.com/b86c730e43083914eb128d7928e57981.png)

Contained BruinNav.exe file runs independently on linux environment as long as mapdata.txt is in same directory

Current version takes user-input locations and provides simpler and clearer directions compared to what was asked for in the original specs.

Original specifications for the project detailed in UCLA Winter 2017 CS-32 Course: http://web.cs.ucla.edu/classes/winter17/cs32/Projects/4/spec.pdf
