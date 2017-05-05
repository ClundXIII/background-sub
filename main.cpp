#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>

using namespace std;

typedef double real;

int main(int argc, char **argv){

    vector<string> argList;

    for (int i=0; i<argc; i++){
        argList.push_back(argv[i]);
    }

    if (argList.size() != 9){
        cout << "Expected 7 args:" << endl
             << "1) filename" << endl
             << "2) last File number" << endl
             << "3) destination filename" << endl
             << "4) background filename" << endl
             << "5) sample spec file" << endl
             << "6) spec intesity of back" << endl
             << "7) integration check filename" << endl
             << "8) Background multiplication factor" << endl;
        return -1;
    }

    string baseFilename = argList.at(1);
    int last_file_num   = stoi(argList.at(2));
    string destinationFilename = argList.at(3);

    vector<real> background;

    ifstream background_file;
    background_file.open(argList.at(4));
    string line_b;
    while (getline(background_file,line_b)){

    	if (line_b.at(0) == '#'){
    		continue;
    	}

    	stringstream streamline;
    	streamline.str(line_b);
        
        real val1, val2;
    	streamline >> val1;
    	streamline >> val2;

    	background.push_back(val2);
    }

    cout << "background: " << background.size() << " datapoints!" << endl;

    background_file.close();

    string::size_type sz;
    real back_intensity = stod(argList.at(6), &sz);
    real back_mul = stod(argList.at(8), &sz);

    vector<real> spec_int;

    ifstream spec_int_file;
    spec_int_file.open(argList.at(5));
    string line_mul_b;
    while (getline(spec_int_file,line_mul_b)){

    	if (line_mul_b == ""){
    		continue;
    	}
    	if (line_mul_b.at(0) == '#'){
    		continue;
    	}

    	stringstream streamline;
    	streamline.str(line_mul_b);
        
        real val1, val2;
        for (int j=0; j<12; j++)
    		streamline >> val1;
    	streamline >> val2;

    	spec_int.push_back(val2);
    }

    cout << "background_mul (spec) \"" << argList.at(5) << "\": " << spec_int.size() << " datapoints!" << endl;

    ofstream int_test_file;
    int_test_file.open(argList.at(7));

    cout << "processing ..." << endl;
	int i=1;
    for (; i<last_file_num; i++){
        stringstream in_filename;
        in_filename << baseFilename << setfill('0') << setw(5) << i << ".sm";

        stringstream out_filename;
        out_filename << destinationFilename << setfill('0') << setw(5) << i << ".sm";

        ifstream in_f;
        ofstream out_f;

        in_f.open(in_filename.str());
        out_f.open(out_filename.str());

        real sum = 0, sum1 = 0;

		//cout << "ration measurement: " << spec_int.at(0)/spec_int.at(i) << " ration background: " << spec_int.at(0)/back_intensity << endl;
        /*int oadfjwe;
        cin >> oadfjwe;*/

        int line_number = 0;
        string line;
        while (getline(in_f,line)){

        	if (line.at(0) == '#'){
        		continue;
        	}

        	stringstream streamline;
        	streamline.str(line);

        	real val1, val2;
        	streamline >> val1;
        	streamline >> val2;

        	//real val3 = back_intensity * val2 - background.at(line_number) * spec_int.at(i);
			real val3 = /*spec_int.at(0) * */( val2 / spec_int.at(i) - back_mul * background.at(line_number) / back_intensity);

        	/*cout << "spec at 0: " << spec_int.at(0) << ", spec int at t:" << spec_int.at(i) << "back at line: " << background.at(line_number) << ", back_intensity: " << back_intensity << endl;

        	cout << "val1: " << val1 << ", val2: " << val2 << ", val3: " << val3 << endl;

        	int oadfjwe;
        	cin >> oadfjwe;*/

        	out_f << val1 << " " << val3 << " " << val2 << endl;
        	out_f.flush();

        	sum += val3;
        	sum1 += val2;

        }
    	if ((i % 50) == 0)
    		cout << "."; cout.flush();


        int_test_file << sum << " " << sum1 << endl;

        in_f.close();

        out_f.close();

    }

    int_test_file.close();

    cout << "files: " << i << endl;

    return 0;
}
