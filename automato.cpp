#include <iostream>
#include <vector>
#include <map>
#include <queue>

#define qtdestados 4
#define qtdsimbolos 3


using namespace std;

vector <string> picotar ( string modelo );

class AFN {
	public:
		string estadoInicial;
		vector <string> estados;
		vector <string> terminais;
		vector <string> estadosfinais; 
		map <string,int> :: iterator it;

		string matriz[qtdestados][qtdsimbolos];

		vector < string > AFD;
		queue < string > estadoparaconversao;

		void addEstados ( string conjuntoDeEstados ){
			estados = picotar(conjuntoDeEstados);
		}

		void addEstadosFinais ( string conjuntoDeEstadosFinais ){
			estadosfinais = picotar(conjuntoDeEstadosFinais);
		}

		void addSimbolos ( string conjuntoDeSimbolos ){
			terminais = picotar(conjuntoDeSimbolos);
		}

		void setEstadoInicial ( string estadoInicial ){
			this->estadoInicial = estadoInicial;
			estadoparaconversao.push(estadoInicial);
		}

		void addTransicao ( string inicio, string fim, string simbolo ){
			if ( matriz[getIndiceEstado(inicio)][getIndiceSimbolo(simbolo)].size() == 0 ){
				matriz[getIndiceEstado(inicio)][getIndiceSimbolo(simbolo)] = fim;
			} else {
				matriz[getIndiceEstado(inicio)][getIndiceSimbolo(simbolo)] += ", " + fim;
			}
		}

		int getIndiceEstado ( string estado ){
			for(int i=0;i<estados.size();i++){
				if ( estados[i] == estado ) return i;
			}
			return 0;
		}
		int getIndiceSimbolo ( string simbolo ){
			for(int i=0;i<terminais.size();i++){
				if ( terminais[i] == simbolo ) return i;
			}
			return 0;
		}

		string pegarPossibilidades ( string estado ){
			map <string, int> mapa;
			string possiveis;
			possibilidades(mapa,estado);

			for(it = mapa.begin();it != mapa.end();it++){
				if ( it != mapa.begin() ) possiveis += ", ";
				possiveis += it->first;
			}
			return possiveis;
		}

		void possibilidades ( map <string,int> & mapa, string estado ){
			vector <string> estadosAlcancaveis = picotar(estado);
			for(int i=0;i<estadosAlcancaveis.size();i++){
				it = mapa.find(estadosAlcancaveis[i]);
				if ( it == mapa.end()){
					mapa.insert( pair<string,int>(estadosAlcancaveis[i],1));
					possibilidades(mapa,matriz[getIndiceEstado(estadosAlcancaveis[i])][getIndiceSimbolo("*")]);
				}
			}
		}

		void imprimirMatriz(){
			for(int i=0;i<estados.size();i++){
				for(int j=0;j<terminais.size();j++){
					cout << estados[i] << "-" << terminais[j] << " = " << matriz[getIndiceEstado(estados[i])][getIndiceSimbolo(terminais[j])] << endl;
				}
				cout << endl;
			}
		}

		string distinct ( string todosestados ){
			map <string,int> mapa;
			map <string,int> :: iterator it;
			string novovalor;

			vector <string> valores = picotar(todosestados);
			for(int i=0;i<valores.size();i++){
				it = mapa.find(valores[i]);
				if ( it == mapa.end() ){
					mapa.insert( pair<string,int>(valores[i],1));
				}
			}
			for(it = mapa.begin();it != mapa.end();it++){
				if ( it != mapa.begin()) novovalor += ", ";
				novovalor += it->first;
			}

			return novovalor;
		}

		string adjacentes ( string estado, string simbolo ){
			string nova = matriz[getIndiceEstado(estado)][getIndiceSimbolo(simbolo)];
			nova += "," + pegarPossibilidades(nova);
			return nova;
		}

		string fatiar ( string estado, string simbolo ){
			vector <string> novo = picotar(estado);
			string retorno;
			for(int i=0;i<novo.size();i++){
				retorno += adjacentes(novo[i],simbolo) + ", "; 
			}
			retorno = distinct(retorno);
			return retorno;
		}

		string verificaEstado ( string estado ){
			vector <string> picotado = picotar(estado);
			string retorno = "";

			if ( picotado.size() == 1 && picotado[0] == estadoInicial ) retorno += "->";

			bool final = false;
			for(int i=0;i<picotado.size();i++){
				for(int j=0;j<estadosfinais.size();j++){
					if ( picotado[i] == estadosfinais[j]) final = true;
				}
			}
			if ( final == true ) return (retorno + "*");
			return retorno;
		}

		void converter () {
			map < string, int > mapa;
			map < string, int> :: iterator it;

			while ( !estadoparaconversao.empty() ){
				string estado = estadoparaconversao.front();
				estadoparaconversao.pop();
				for(int i=0;i<terminais.size();i++){
					if ( terminais[i] != "*" ){
						string final = fatiar(estado,terminais[i]);
						string query = verificaEstado(estado) + "{ " + estado + " } (" + terminais[i] + ") = {" + final + "}";
						it = mapa.find(query);
						if ( it == mapa.end() && estado != "" ){
							estadoparaconversao.push(final);
							mapa.insert( pair<string,int> (query,1));
						}
					}
				}
			}
			for(it = mapa.begin(); it!= mapa.end();it++){
				cout << it->first << endl;
			}
		}
};

int main () {
	AFN automato;

	automato.addEstados("q1,q2,q3,q4");
	automato.setEstadoInicial("q1");
	automato.addEstadosFinais("q4");
	automato.addSimbolos("0,1,*");

	automato.addTransicao("q1","q1","0");
	automato.addTransicao("q1","q1","1");
	automato.addTransicao("q1","q2","1");
	automato.addTransicao("q2","q3","0");
	automato.addTransicao("q2","q3","*");
	automato.addTransicao("q3","q4","1");
	automato.addTransicao("q4","q4","0");
	automato.addTransicao("q4","q4","1");

	automato.converter();
}

vector <string> picotar ( string modelo ) {
	vector <string> retorno;
	string aux = "";

	for(int i=0;i<modelo.size();i++){
		if ( modelo[i] == ',' || modelo[i] == ' ' ){
			if ( aux != "" ) retorno.push_back(aux);
			aux = "";
		} else { 
			aux += modelo[i];
		}

		if ( i == modelo.size() - 1 ){
			if ( aux != "" ) retorno.push_back(aux);
		}
	}
	return retorno;
}