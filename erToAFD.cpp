#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define qtdsimbolos 4
//a,b,c,*

vector <char> simbolos;

int ultimo = 0;

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

class estado {
public:
	int inicial;
	vector <int> finais;
	vector < vector<string> > vetor;

	estado ( char valor ) {
		vector <string> aux;
		vector <string> aux2;

		for(int i=0;i<qtdsimbolos;i++){
			aux2.push_back("0");
		}

		for(int i=0;i<qtdsimbolos;i++){
			if ( simbolos[i] == valor ){
				aux.push_back("2");
				inicial = 1;
				finais.push_back(2);
			} else {
				aux.push_back("0");
			}
		}
		vetor.push_back(aux);
		vetor.push_back(aux2);
	}

	estado () {

	}

	void transforma ( estado & a, int valor ){
		a.inicial += valor;
		for(int i=0;i<a.finais.size();i++){
			a.finais[i] += valor;
		}

		for(int i=0;i<a.vetor.size();i++){
			for(int j=0;j<qtdsimbolos;j++){
				if ( a.vetor[i][j] != "0" ){
					vector <string> u = picotar(a.vetor[i][j]);
					string nova = "";
					for(int k=0;k<u.size();k++){
						if ( k != 0 ) nova += ",";
						nova += to_string(stoi(u[k])+valor);
					}
					a.vetor[i][j] = nova;
				}
			}
		}
	}

	void concatena ( estado a ){
		transforma(a,vetor.size());

		for(int i=0;i<finais.size();i++){
			vetor[finais[i]-1][qtdsimbolos-1] = to_string(a.inicial);
		}

		finais = a.finais;
		for(int i=0;i<a.vetor.size();i++){
			vetor.push_back(a.vetor[i]);
		}
	}

	void uniao ( estado a ){
		transforma(a,vetor.size());
		for(int i=0;i<a.vetor.size();i++){
			vetor.push_back(a.vetor[i]);
		}

		vector <string> aux;
		for(int i=0;i<qtdsimbolos;i++){
			aux.push_back("0");
		} 
		string nova = to_string(inicial) + "," + to_string(a.inicial);
		aux[qtdsimbolos-1] = nova;

		vetor.push_back(aux);

		inicial = vetor.size();

		for(int i=0;i<a.finais.size();i++){
			finais.push_back(a.finais[i]);
		}
	}

	void loop () {
		for(int i=0;i<finais.size();i++){
			if ( vetor[finais[i]-1][qtdsimbolos-1].size() == 1 && vetor[finais[i]-1][qtdsimbolos-1][0] == '0'){
				vetor[finais[i]-1][qtdsimbolos-1]  = to_string(inicial);
			} else {
				vetor[finais[i]-1][qtdsimbolos-1] += ("," + to_string(inicial));
			}
		}
	}

	void imprime (){
		for(int i=0;i<vetor.size();i++){
			cout << "Q" << i+1 << ": ";
			for(int j=0;j<qtdsimbolos;j++){
				cout << vetor[i][j] << " ";
			}
			cout << endl;
		}

		cout << "Inicial: " << inicial << endl;
		cout << "Finais: ";
		for(int i=0;i<finais.size();i++){
			if ( i != 0 ) cout << ", ";
			cout << finais[i];
		} cout << endl;
	}

	void converte (){
		string estados = "";
		for(int i=0;i<vetor.size();i++){
			if ( i != 0 ) estados += ",";
			estados += "q" + to_string(i+1);
		}
		string expressao = "automato.addEstados(\"" + estados + "\");";
		cout << expressao << endl;
		expressao = "automato.setEstadoInicial(\"q" + to_string(inicial) + "\");";
		cout << expressao << endl;

		estados = "";
		for(int i=0;i<finais.size();i++){
			if ( i != 0 ) estados += ",";
			estados += "q" + to_string(finais[i]);
		}
		expressao = "automato.addEstadosFinais(\"" + estados + "\");";
		cout << expressao << endl;

		estados = "";
		for(int i=0;i<simbolos.size();i++){
			if ( i != 0 ) estados += ",";
			estados += simbolos[i];
		}
		estados += ",*";
		expressao = "automato.addSimbolos(\"" + estados + "\");";
		cout << expressao << endl;

		for(int i=0;i<vetor.size();i++){
			for(int j=0;j<qtdsimbolos;j++){
				if ( vetor[i][j][0] != '0' ){
					vector <string> estadosadjacentes = picotar(vetor[i][j]);
					for(int k=0;k<estadosadjacentes.size();k++){
						expressao = "";
						expressao += "automato.addTransicao(\"q" + to_string(i+1) + "\",\"q" + estadosadjacentes[k] + "\",\"";
						string simbolodatransicao;
						if ( j == qtdsimbolos-1 ){
							simbolodatransicao = "*";
						} else {
							simbolodatransicao = simbolos[j];
						}
						expressao += simbolodatransicao + "\");";
						cout << expressao << endl;
					}
				}
			}
		}		
	}
};

//função para transformar a string em um árvore
estado separaString ( string expressao ){

	if ( expressao.size() == 3 ) return estado(expressao[1]);

	int flag = 0;
	string pequena = "";
	bool uniao = false;
	bool primeira = true;
	estado final('8');

	for(int i=0;i<expressao.size();i++){
		if ( expressao[i] == 'U') uniao = true;
		if ( expressao[i] == '(' ) flag++;
		if ( expressao[i] == ')' ) flag--;
		
		if ( expressao[i] != 'U' && expressao[i] != '*' ) pequena += expressao[i];
		else if ( flag != 0 ) pequena += expressao[i];

		if ( expressao[i] == '*') final.loop();
		else if ( flag == 0 && pequena.size() != 0 ){
			pequena = pequena.substr(1,pequena.size()-2);
			if ( pequena.size() >= 3 ) {
				if ( primeira == true ) {
					final = separaString(pequena);
						//cout << "FINAL: " << pequena << endl;
						//final.imprime();
						//cout << endl;
					primeira = false;
				} else {
					if ( uniao ) final.uniao(separaString(pequena));
					else final.concatena(separaString(pequena));
						//cout << "FINAL: " << pequena << endl;
						//final.imprime();
						//cout << endl;
				}
			} else {
				if ( primeira == true ){
					final = estado(pequena[0]);
					primeira = false;
				} else {
					if ( uniao ) final.uniao(estado(pequena[0]));
					else final.concatena(estado(pequena[0]));
				}
				//cout << "FINAL: "<< pequena << endl;
				//final.imprime();
				//cout << endl;
			}
			pequena = "";
		}
	}
	return final;
}

void pegarSimbolosTerminais ( string expressao ){
	int valor;
	for(int i=0;i<expressao.size();i++){
		if ( expressao[i] != 'U' && expressao[i] != '*'){
			valor = expressao[i];

			bool coloca = true;
			for(int j=0;j<simbolos.size();j++){
				if ( simbolos[j] == expressao[i] ) coloca = false;
			}

			if ( coloca == true ){
				if ( valor >= 48 && valor <= 57 ) simbolos.push_back(expressao[i]);
				if ( valor >= 65 && valor <= 90 ) simbolos.push_back(expressao[i]);
				if ( valor >= 97 && valor <= 122 ) simbolos.push_back(expressao[i]);
			}
		}
	}
}

int main () {
	string expressaoRegular;

	cout << "Insira a expressão regular: " << endl;
	cin >> expressaoRegular;

	pegarSimbolosTerminais(expressaoRegular);
	estado final = separaString(expressaoRegular); 
	final.imprime();
	cout << endl << endl;
	final.converte();
}