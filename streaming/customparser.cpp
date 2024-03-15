// Complete me
#include "customparser.h"
#include <iostream>
#include <sstream>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";

// To Do - Complete this function
void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    // Erase any old contents
    content.clear();
    users.clear();

    // TO DO - Add your code below.
    // get rid of whitespcae
    string wspace = " \n\r\t\f\v";
    int contents = 0;
    is >> contents;
    if (is.fail()) {
        throw ParserError(error_msg_1);
    }

    // initialize the variables
    int style = -1;
    int id = 0;
    int numReviews = 0;
    int totalStars = 0;
    int rating = 0;
    int movies = 0;
    string name = "";

    // for loop that goes through all the contents
    for (int i = 0; i < contents; i++) {

        // have to make sure that there are cases if id failes
        is >> id;
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }

        // have the case for if the style fails
        is >> style;
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }

        is.ignore(1000, '\n');

        // case if the name fails
        getline(is, name);
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }
         
        size_t indexStart = name.find_first_not_of(wspace);
        // remove the front tab
        name = name.substr(indexStart); 

        
        is >> numReviews;
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }

        // case for if the totalstars fails
        is >> totalStars;
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }
        
        // case for if the rating fails
        is >> rating;
        if (is.fail()) {
            throw ParserError(error_msg_2);
        }

        // if there's sequels to the movies
        if (style == 1) {
          is >> movies;
          if (is.fail()) {
              throw ParserError(error_msg_2);
          }
        }

        // go to the next line
        is.ignore(1000, '\n');

        // initialize the string user that gets all the users
        string user = "";
        getline(is, user);
        if (is.fail()) {
          throw ParserError(error_msg_2);
        }
        // get the front tab
        indexStart = user.find_first_not_of(wspace);
        // if there are users
        if (indexStart != string::npos) { 
          user = user.substr(indexStart);
        }

        if (style == 0) {
            // dynamic allocation for movie
            Content* temp = new Movie(id, name, numReviews, totalStars, rating);

            // add the users
            while (user.length() > 0) {
              int spaceIndex = user.find(' ');
              // if it's the last user
              if (spaceIndex == -1) {
                temp->addViewer(user);
                user = "";
              }
              // if there's only one viewer is the else statement
              else {
                temp->addViewer(user.substr(0, spaceIndex));
                user = user.substr(spaceIndex + 1);
              }
            }

            content.push_back(temp);
        }
        else {
            // if there are multiple sequels of movies
            // dynamic allocation of movies
            Content* temp = new Series(id, name, numReviews, totalStars, rating, movies);


            // adds the users
            while (user.length() > 0) {
              int spaceIndex = user.find(' ');
              // same thing as earlier
              if (spaceIndex == -1) {
                temp->addViewer(user);
                user = "";
              }
              // same case as earlier with the last viewer
              else {
                temp->addViewer(user.substr(0, spaceIndex));
                user = user.substr(spaceIndex + 1);
              }
            }

            content.push_back(temp);
        }
    }


    // add the users
    // initialize the string user
    string user = "";
    int ratingLimit = 0;
    while (is >> user) {
      
      is >> ratingLimit;
      if (is.fail()) {
        throw ParserError(error_msg_3);
      }
      is.ignore(1000, '\n');

      // dynamic allocation of the new user
      User* u = new User(user, ratingLimit);


      // make all the users as contentID
      getline(is, user); 
      int contentID = 0;
      stringstream allIDs(user);
      while (allIDs >> contentID) {
        u->addToHistory(contentID);
      }
      // reset the stringstreams
      allIDs.clear();
      allIDs.str("");
      users.push_back(u);
    }

}
