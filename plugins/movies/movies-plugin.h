#include"../../include/plugins/plugin.h"

/* Movies implements the Plugin abstract class. */
class Movies: Plugin {

	public:
	/***************/
	/* We basically check whether this is the right plugin to be called. */
	int getInputComparisonScore(vector<string> input) {
		int length = input.size();

		for(int iter=0; iter<length; iter++) {
			if(input[iter] == "movies" || input[iter] == "movie") return 1;
			else if(input[iter] == "films" || input[iter] == "film") return 1;
		}
		return 0;
	}

	/***************/
	/* Takes the data (movies vector) given and displays it in a formatted way. */
	void displayAnswer(vector<string> input, string name, bool onlyPlugins) {

		vector<int> options = parseOptionsFromInput(input);
		
		if(options[0] != 20) { /* If there is a valid answer... */
			
			saveHTMLFromURL(getRequestURL(options));
			vector<string> films = getDataFromHTML();

			/* We format the starting according to the given mode. */
			if(onlyPlugins) cout<<name<<": The top films in the ";
			else cout<<"\n"<<name<<": Additionally, according to Rotten Tomatoes, these are the top films in the ";

			/* We fill in the genres. */
			switch(options[0]) {
				case 1: cout<<"'Adventure & Action'"; break;
				case 2: cout<<"'Animation'"; break;
				case 3: cout<<"'Comedy'"; break;
				case 4: cout<<"'Drama'"; break;
				case 5: cout<<"'Horror'"; break;
				case 6: cout<<"'Kids & Family'"; break;
				case 7: cout<<"'Music & Musicals'"; break;
				case 8: cout<<"'Mystery'"; break;
				case 9: cout<<"'Suspense & Thriller'"; break;
				case 10: cout<<"'Romance'"; break;
				case 11: cout<<"'Sci-Fi'"; break;
				case 12: cout<<"'Sports'"; break;
				case 13: cout<<"'War'"; break;
				case 16: cout<<"'All'"; break;
				case 14: cout<<"'Oscar-Winning - Best Feature Film' genre. These films would have won only the year later, as the Academy Awards mostly feature films made the previous year. Movies from this "; break;
				case 15: cout<<"'Kollywood/Tamil'"; break;
				case 17: cout<<"'BAFTA-Winning - Best Feature Film'"; break;
				case 18: cout<<"'Disney Original Movies'"; break;
				case 19: cout<<"'Malayalam'"; break;
			}
			

			/* If year is present, we fill in the year. */
			if(options[1] != 0) cout<<" genre from the year "+to_string(options[1])+" are shown below. ";
			else cout<<" genre are shown below, along with the year of their release. ";

			cout<<" Information is sourced live from criticker.com!\n";
			formatTextIntoCols(2,films); //We format the movie titles into two columns.
			cout<<"\n";
		}

	}

	private:
	
	/***************/
	/* Given the vector input, we parse options (parameters) for the URL requests. */
	vector<int> parseOptionsFromInput(vector<string> input) {

		/* The first option is genre (awards are genres too). Second option is year. */
		string genres[] = {"adventure","action", "animation","comedy","drama","horror","kids","family","musical","music","mystery","suspense","thriller","romance","romantic","science","sci","fantasy","sports","fitness","game","war","oscars","oscar","award","awards","tamil","kollywood","bafta","disney","plus","malayalam","mallu","mollywood","all"};
		int genreOptions[] = {1,1,2,3,4,5,6,6,7,7,8,9,9,10,10,11,11,11,12,12,12,13,14,14,14,14,15,15,17,18,18,19,19,19,16};
		int noOfGenres = 35;

		vector<int> options;

		/* We shall first try and see if a genre exists in the sentence. */
		int size = input.size();
		for(int iter=0; iter<size; iter++) {
			for(int jiter=0; jiter<noOfGenres; jiter++) {
				if(input[iter].find(genres[jiter]) != string::npos){ 
					options.push_back(genreOptions[jiter]);
					break;
				}
			}
			if (options.size() != 0) break; /* If a genre exists, then break. */
		}
		if(options.size() == 0) options.push_back(16); /* If no genres, then we suggest 'all'. */

		/* If no genre existed, then we search for the year. */
		for(int iter=0; iter<size; iter++) {
			if(isYear(input[iter])) {
				options.push_back(stoi(input[iter]));
				return options;
			}
		}
		if(options.size() == 1) options.push_back(0); /* If no year, then we suggest all. */
		
		return options;
	}

	/***************/
	/* Using the options (parameters) given, we formulate the request URL. */
	string getRequestURL(vector<int> options) {

		string baseURL = "'https://www.criticker.com/films/?filter=";

		switch(options[0]) {
			case 1: baseURL.append("gy12x13zi1zf1900zt2030zor'"); break; //action
			case 2: baseURL.append("gy18zi1zf1900zt2030zor'"); break; //animated
			case 3: baseURL.append("gy2zi1zf1900zt2030zor'"); break; //comedy
			case 4: baseURL.append("gy3zi1zf1900zt2030zor'"); break; //drama
			case 5: baseURL.append("gy15zi1zf1900zt2030zor'"); break; //horror
			case 6: baseURL.append("gy10zi1zf1900zt2030zor'"); break; //kids
			case 7: baseURL.append("gy41x9zi1zf1900zt2030zor'"); break; //music
			case 8: baseURL.append("gy17zi1zf1900zt2030zor'"); break; //mystery
			case 9: baseURL.append("gy6zi1zf1900zt2030zor'"); break; //thriller/suspense
			case 10: baseURL.append("gy1zi1zf1900zt2030zor'"); break; //romance
			case 11: baseURL.append("gy5zi1zf1900zt2030zor'"); break; //scifi
			case 12: baseURL.append("gy34zi1zf1900zt2030zor'"); break; //sport
			case 13: baseURL.append("gy14zi1zf1900zt2030zor'"); break; //war
			case 14: baseURL.append("f1900zt2030ze3139zod'"); break; //oscar
			case 15: baseURL.append("f1900zt2030ze3075zor'"); break; //tamil
			case 16: baseURL.append("i1zf1900zt2030zor'"); break; //all
			case 17: baseURL.append("f1900zt2030ze10797zod'"); break; //bafta
			case 18: baseURL.append("f1900zt2030ze42853zor'"); break; //disney/disney+
			case 19: baseURL.append("f1900zt2030ze2908zor'"); break; //malayalam
		}

		/* For year, we replace year in URL with our own. */
		if(options[1] != 0 && options[0] == 14) {
			baseURL.replace(baseURL.find("1900"),4,to_string(options[1]-1));
			baseURL.replace(baseURL.find("2030"),4,to_string(options[1]-1));
		}
		else if(options[1] != 0) {
			baseURL.replace(baseURL.find("1900"),4,to_string(options[1]));
			baseURL.replace(baseURL.find("2030"),4,to_string(options[1]));
		}
		
		return baseURL;
	}

	/***************/
	/* We save the request response HTML to a file. */
	void saveHTMLFromURL(string url) {
		string cmd = "curl -s " + url + " > data.txt";
		system(cmd.c_str());
	}	

	/****************/
	/* We parse the response HTML in the file to get our data (movie vectors). */
	vector<string> getDataFromHTML() {
		
		ifstream file("data.txt");
		string word;
		string movie = "";

		vector<string> movies;

		while(!file.eof()) {
			file>>word;
			
			if(word == "class='fl_titlelist_link'") {

				int flag = 0;
				char character;

				while(true) {
					file.get(character);
					if(character == '<') break;
					if(flag == 1) movie.push_back(character);
					if(character == '>') flag = 1;	
				}

				movies.push_back(movie);
				movie = "";
			
			}

		}
		
		file.close();
		return movies;
	}

};
