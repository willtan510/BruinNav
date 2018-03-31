# BruinNav

C++ program that builds navigation system that loads and indexes OpenStreetMap geospatial data, and implements A* search algorithm to output optimal turn by turn directions from user-input source to destination. mapData.txt stores the OpenStreetMap data of UCLA, Westwood, West Los Angeles, West Hollywood, Brentwood, and Santa Monica. 


After the user inputs valid attraction as the starting point and a valid attraction as the destination, the program outputs a cardinal direction along with a distance with a precision of .001 miles to proceed walking or a directional turn if changing streets.

EXAMPLE: Directions from UCLA dorm building Rieber Terrace to popular ice cream store in Westwood, Diddy Riese:
![example-image](https://gyazo.com/9c98615ca0ce2ebebdcb99889a475496)



Current version takes user-input locations and provides simpler and clearer directions compared to what was asked for in the original specs.
Original specifications for the project detailed from UCLA CS-32 Course: http://web.cs.ucla.edu/classes/winter17/cs32/Projects/4/spec.pdf
