#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
class line_writer{
	std::ofstream *line;
	public:
		line_writer(const std::string& path){
			line = new std::ofstream(path, std::ios::out);
		}
		~line_writer(){
			line -> close();
			delete line;
			std::cout << "plik zostal pomyslnie zamkniety!\n";
		}

		template <typename T> line_writer& write(T data){
			*line << data << "\n";
			return *this;
		}

};

int main(){
	std::string name;
	std::cout << "Podaj nazwe pliku: ";
	std::cin >> name;
	auto writer = std::make_shared<line_writer>(name);
	std::vector<std::shared_ptr<line_writer>> vec;
	for(int i = 0; i < 20; i++)
		vec.push_back(writer);
	for(int i = 0; i < 20; i++)
		vec[i] -> write(i);
	
	std::shared_ptr<line_writer> notinvec = writer;
	notinvec->write("jestem poza wektorem? moge tak? :o");
	return 0;
		
}
