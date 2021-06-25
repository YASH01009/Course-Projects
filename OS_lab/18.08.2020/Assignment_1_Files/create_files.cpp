/**
 * C program to create a file and write data into file.
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define FILE_NO 10000

int create_file(int r)
{

    /* Create a new file */
    ofstream Myfile("data/file" + to_string(r) + ".txt");

    /* Variable to store user content */
    string data = "I am a second year under graduate pursuing my B.Tech degree in the field of Computer Science and Engineering form Indian \
    Institute of Technology (IIT) Bhubhaneswar. I have completed the Machine Learning course by Dr.Andrew Ng with lots of interest in \
    a span of two months managing my academics from my institute. For that course, I have enrolled myself without opting for a certificate. \
    I really can not afford for this course but I find a lot of interest for being a student of Dr.Andrew Ng. I also find great interest in \
    the course content. It would be a great help from Coursera if it can provide me with access for this course, which I assure will not be \
    wasted and I promise I will not misuse this great opportunity from the team of Coursera. I request you to kindly consider this request \
    of mine, which would help in a great manner. I can not afford for these specialization courses, for which I need your kind help. \
    This scholarship when approved provides me great help for me to pursue in my career interests. I also completed the first part of the specialization \
    with a great interest with your kind help. As I do not have any sources of income beyond my parents, affording such an amount is highly out of \
    my abilities. I am still a student and I can not afford but can surely make a very good help of this course, if allowed to access the course content.";

    /* Write data to file */
    Myfile << data;

    /* Close file to save file data */
    Myfile.close();


    /* Success message */
    cout << "Successfully created " << r << "files :)" << endl;


    return 0;
}


int main() {

    /* Creating multiple files using above function */
    for(int i = 1; i <= FILE_NO; i++)
        create_file(i);

    return 0;
}