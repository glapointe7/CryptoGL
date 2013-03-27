/* 
 * File:   Data.h
 * Author: gabriel
 *
 * Created on March 23, 2013, 2:48 PM
 */

#ifndef DATA_H
#define	DATA_H

#include <string>

using namespace std;

class Data 
{
    public:
        Data();
        Data(const Data& orig);
        virtual ~Data();
        
        static string load(const string filename, bool keep_space = false);
        static void save(const string filename, const string data);
    
    private:
        string encoded_file;
        string decoded_file;
};

#endif	/* DATA_H */

