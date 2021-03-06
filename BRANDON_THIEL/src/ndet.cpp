#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <list>
#include <cassert>
#include <utility>

// pour la seconde partie du projet
#include "expression_rationnelle.hpp"
#include "parser.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

const unsigned int ASCII_A = 97;
const unsigned int ASCII_Z = ASCII_A + 26;
const bool         DEBUG = false;

typedef size_t                            etat_t;
typedef unsigned char                     symb_t;
typedef set< etat_t >                     etatset_t;
typedef vector< vector< etatset_t > >     trans_t;
typedef vector< etatset_t >               epsilon_t;
typedef map< etatset_t, etat_t >          map_t;

//nous sert à mettre du vide, pour l'initialisation de tableau.
const vector<etatset_t> vide;
const etatset_t videSet;
////////////////////////////////////////////////////////////////////////////////

struct sAutoNDE{
  // caractéristiques
	size_t nb_etats;
	size_t nb_symbs;
	size_t nb_finaux;

	etat_t initial;
  // état initial

	etatset_t finaux;
  // états finaux : finaux_t peut être un int*, un tableau dynamique comme vector<int>
  // ou une autre structure de donnée de votre choix.

	trans_t trans;
  // matrice de transition : trans_t peut être un int***, une structure dynamique 3D comme vector< vector< set<int> > >
  // ou une autre structure de donnée de votre choix.

	epsilon_t epsilon; 
  // transitions spontanées : epsilon_t peut être un int**, une structure dynamique 2D comme vector< set<int> >
  // ou une autre structure de donnée de votre choix.
};

////////////////////////////////////////////////////////////////////////////////

//Prototypes
ostream& operator<<(ostream& out, const sAutoNDE& at);
bool FromFile(sAutoNDE& at, string path){
	ifstream myfile(path.c_str(), ios::in); 
  //un flux d'entree obtenu à partir du nom du fichier
	string line;
  // un ligne lue dans le fichier avec getline(myfile,line);
	istringstream iss;
  // flux associé à la chaine, pour lire morceau par morceau avec >> (comme cin)
	etat_t s(0), t(0);
  // deux états temporaires
	symb_t a(0);
  // un symbole temporaire

	if (myfile.is_open()){
    // la première ligne donne 'nb_etats nb_symbs nb_finaux'
		do{ 
			getline(myfile,line);
		} while (line.empty() || line[0]=='#');
    // on autorise les lignes de commentaires : celles qui commencent par '#'
		iss.str(line);
		if((iss >> at.nb_etats).fail() || (iss >> at.nb_symbs).fail() || (iss >> at.nb_finaux).fail())
			return false;
    // la deuxième ligne donne l'état initial
		do{ 
			getline (myfile,line);
		} while (line.empty() || line[0]=='#');    
		iss.clear();
		iss.str(line);
		if((iss >> at.initial).fail())
			return -1;

    // les autres lignes donnent les états finaux
		for(size_t i = 0; i < at.nb_finaux; i++){
			do{ 
				getline (myfile,line);
			} while (line.empty() || line[0]=='#');
			iss.clear();
			iss.str(line);
			if((iss >> s).fail())
				continue;
//        cerr << "s= " << s << endl;
			at.finaux.insert(s);
		}     

    // on alloue les vectors à la taille connue à l'avance pour éviter les resize dynamiques
		at.epsilon.resize(at.nb_etats);
		at.trans.resize(at.nb_etats);
		for(size_t i=0;i<at.nb_etats;++i)
			at.trans[i].resize(at.nb_symbs);   

  // lecture de la relation de transition 
		while(myfile.good()){
			line.clear();
			getline (myfile,line);
			if (line.empty() && line[0]=='#')
				continue;
			iss.clear();
			iss.str(line); 

      // si une des trois lectures echoue, on passe à la suite
			if((iss >> s).fail() || (iss >> a).fail() || (iss >> t).fail() || (a< ASCII_A ) || (a> ASCII_Z ))
				continue; 

      //test espilon ou non
			if ((a-ASCII_A) >= at.nb_symbs){
//        cerr << "s=" << s<< ", (e), t=" << t << endl;
// TODO: remplir epsilon
				at.epsilon[s].insert(t);
			}
			else{
//        cerr << "s=" << s<< ", a=" << a-ASCII_A << ", t=" << t << endl;
// TODO: remplir trans
				at.trans[s][a-ASCII_A].insert(t);
			}
		}

		myfile.close();
		return true; 
	}
	return false;
  // on ne peut pas ouvrir le fichier
}


// -----------------------------------------------------------------------------
// Fonctions à compléter pour la première partie du projet
// -----------------------------------------------------------------------------


bool EstDeterministe(const sAutoNDE& at){
  //Est deterministe si et seulement si une transion donne vers un seul état (ex: {Q1,Q2} --a--> {Q2,Q3,Q5}).

	for(unsigned int i = 0; i < at.epsilon.size(); i++){
		if(at.epsilon[i].size() > 1){
      return false; //car si au moins un ensemble vide en transition alors l'automate n'est pas déterministe.
  }
}

for(unsigned  int i = 0; i < at.nb_etats; i++){
	for(unsigned int j = 0; j < at.trans[i].size(); j++){
		if(at.trans[i][j].size() > 1){
			return false;
		}
	}

}

return true;
}

////////////////////////////////////////////////////////////////////////////////

void Fermeture(const sAutoNDE& at, etatset_t& e){
  // Cette fonction clot l'ensemble d'états E={e_0, e_1, ... ,e_n} passé en
  // paramètre avec les epsilon transitions
  /*qui ajoute à l'ensemble d'états e tous ceux qui sont accessibles via des transitions spontanées. On pourra s'inspirer par exemple de l'algorithme de Warshall : https://fr.wikipedia.org/wiki/Algorithme_de_Warshall*/

  for(etatset_t  t : at.epsilon){ // chaque transition sont stocké dans e.
  	e.insert(t.begin(), t.end());
  }
}

////////////////////////////////////////////////////////////////////////////////

etatset_t Delta(const sAutoNDE& at, const etatset_t& e, symb_t c){
  //sur la base de celle pour le cas sans transitions spontanées,
  // définir cette fonction en utilisant Fermeture

  /*  
    parcourir e, pour chaque tu prend etat dans at.trans, c = 2ieme indice du parcour
    Pour un état on calcule chaque état suivant possible à partir du symbole c;
  */
	etatset_t eDelta;

	for(etat_t temp : e){
		eDelta.insert(at.trans[temp][c].begin(), at.trans[temp][c].end());
	}

	return eDelta;
}

////////////////////////////////////////////////////////////////////////////////
sAutoNDE Determinize(const sAutoNDE& at){
	sAutoNDE r;

	r.nb_symbs = at.nb_symbs;

	size_t nb_etats;
	size_t nb_finaux;

	etat_t initial;
	etatset_t finaux;
	trans_t trans;
	epsilon_t epsilon;

  etatset_t e; //tableau des epsilon cloture

  Fermeture(at, e);

  for(etat_t t : e){//Pour chaque états.

    for(int i = 0 ;  i < at.nb_symbs; i++){//Pour chaque symbole d'un état.
    	etatset_t temp;
      temp = Delta(at, e, ASCII_A + i);//ASCII_A + i renvoi le symbole.
  }
}
  return r; // renvoi le graph déterminisé.
}

////////////////////////////////////////////////////////////////////////////////

bool Accept(const sAutoNDE& at, string str){
  //fait marcher l'automate et accept si tombe sur un état final
	size_t i = 0;
	sAutoNDE aut = Determinize(at);
	etat_t et = aut.initial;
	while(i<=str.size()-1){
		if(!aut.trans[et][str[i]-ASCII_A].empty()) {
			et = *(aut.trans[et][str[i]-ASCII_A].begin());
			i++;
		}
		else return false;
	}

	if (aut.finaux.find(et) != aut.finaux.end() )
		return true;
	else 
		return false;

}

////////////////////////////////////////////////////////////////////////////////

ostream& operator<<(ostream& out, const sAutoNDE& at){
	out << at.nb_etats << " " << at.nb_symbs << " " << at.nb_finaux << "\n" << at.initial << "\n\n";

	for(std::set<etat_t>::iterator it = at.finaux.begin() ; it != at.finaux.end() ; ++it ){
		out << *it << "\n";
	}

	out << "\n";

	for (unsigned int i = 0 ; i < at.epsilon.size() ; i++){
		for (std::set<etat_t>::iterator it = at.epsilon[i].begin() ; it != at.epsilon[i].end() ; ++it){
			out << i << " " << static_cast<char>(at.nb_symbs + 1) << " " << *it << "\n";
		}
	}

	for (unsigned int i = 0 ; i < at.trans.size() ; i++){
		for (unsigned int j = 0 ; j < at.trans[i].size() ; j++){
			for (std::set<etat_t>::iterator it = at.trans[i][j].begin(); it != at.trans[i][j].end(); ++it){
				out << i << " " << (char)(j+ASCII_A) << " " << *it << "\n";
			}
		}
	}
	return out;
}

////////////////////////////////////////////////////////////////////////////////

bool ToGraph(sAutoNDE& at, string path){
	std::ofstream outfile (path, std::ofstream::out);
	outfile << "diagraph finite_state_machine {\n\trankdir=LR;\n\tsize=\"10,10\"\n\n node [shape = doublecircle]; ";
	for(std::set<etat_t>::iterator it = at.finaux.begin(); it != at.finaux.end(); ++it){
		outfile << *it << " ";
	}
	outfile << ";\n\tnode [shape = point ]; q;\n\tnode [shape = circle];\n\tq -> " << at.initial << ";\n";
	for (unsigned int i = 0 ; i < at.trans.size() ; i++){
		for (unsigned int j = 0 ; j < at.trans[i].size() ; j++){
			for (std::set<etat_t>::iterator it = at.trans[i][j].begin(); it != at.trans[i][j].end(); ++it){
				outfile << "\t" << i << " -> " << *it << " [label = \""<< (char)(j+ASCII_A) << "\"];\n";
			}
		}
	}
	outfile.close();
	return false;
}


// -----------------------------------------------------------------------------
// Fonctions à compléter pour la seconde partie du projet
// -----------------------------------------------------------------------------

sAutoNDE Append(const sAutoNDE& x, const sAutoNDE& y){
  // fonction outil : on garde x, et on "ajoute" trans et epsilon de y
  // en renommant ses états, id est en décallant les indices des états de y
  // de x.nb_etats 
	assert(x.nb_symbs == y.nb_symbs);
	sAutoNDE r;

  //TODO définir cette fonction

	return r;
}

////////////////////////////////////////////////////////////////////////////////

sAutoNDE Union(const sAutoNDE& x, const sAutoNDE& y){
	assert(x.nb_symbs == y.nb_symbs);
	sAutoNDE r = Append(x, y);

  //TODO définir cette fonction

	return r;
}

////////////////////////////////////////////////////////////////////////////////

sAutoNDE Concat(const sAutoNDE& x, const sAutoNDE& y){
	assert(x.nb_symbs == y.nb_symbs);
	sAutoNDE r = Append(x, y);

  //TODO définir cette fonction

	return r;
}

////////////////////////////////////////////////////////////////////////////////

sAutoNDE Complement(const sAutoNDE& x){
  //TODO définir cette fonction

	return x;
}

////////////////////////////////////////////////////////////////////////////////

sAutoNDE Kleene(const sAutoNDE& x){
  //TODO définir cette fonction

	return x;
}

////////////////////////////////////////////////////////////////////////////////

sAutoNDE Intersection(const sAutoNDE& x, const sAutoNDE& y){
  //TODO définir cette fonction

	return x;

}

////////////////////////////////////////////////////////////////////////////////

sAutoNDE ExpressionRationnelle2Automate(string expr){
	cout << "Construction d'un automate à partir d'une expression rationnelle\n";
	cout << "  Expression en entrée (string) : " << expr << endl;

	sExpressionRationnelle er = lit_expression_rationnelle(expr);

	cout << "  Expression en entrée (ASA)    : " << er << endl;

	sAutoNDE r;

  //TODO définir cette fonction

	return r;
}

////////////////////////////////////////////////////////////////////////////////
string Automate2ExpressionRationnelle(sAutoNDE at){//Bryan
	cout << "Construction d'une expression rationnelle à partir d'un automate\n";

	string expr;

  	//Etape 1 : arrangé l'automate. A Tester
	sAutoNDE atPrime;//M'

	atPrime.nb_symbs = at.nb_symbs;
 	atPrime.nb_etats = at.nb_etats + 2;//M' = n+2 états
 	atPrime.nb_finaux = 1;

 	atPrime.initial = 0;

 	atPrime.trans.push_back(vide);//permet d'init la table


 	int i = 1;
 	int j = ASCII_A;

 	//Remplissage de la table des états.
  	for(vector<etatset_t> temp : at.trans){//pour chaque états.

  		atPrime.trans.push_back(vide);
  		for(etatset_t transi : temp){//pour chaque transition.

  			atPrime.trans[i].push_back(videSet);
  			for(etat_t t : transi){//etat de ces transition.

  				atPrime.trans[i][j].insert(t+1);
  			}
  			j++;
  		}
  		j = ASCII_A;
  		i++;
  	}

  	i = 1;

  	atPrime.epsilon[atPrime.initial].insert(1);//insertion de l'épsilon transition de l'état initial à l'état 1.

  	//Remplissage de la table des epsilon transition.
  	for(etatset_t eTemp : at.epsilon){
  		for(etat_t t : eTemp){
  			atPrime.epsilon[i].insert(t+1);
  		}
  		i++;
  	}

  	for(etat_t fTemp : at.finaux){
  		atPrime.epsilon[atPrime.nb_etats-1].insert(fTemp+1);
  	}


  	//Remplissage de la table des etats finaux.
  	atPrime.finaux.insert(atPrime.nb_etats-1);

  	//Etape 2 : le caractériser. A Coder

  	//R = mot restant sur l'automate, i = état de départ, j= état d'arrivé, K = présedent état retiré de la liste.

  	//attention 0 et dernier état ne sera pas caract.
  	for(i = 1; i < atPrime.nb_etats-1; i++){
  		if(atPrime.trans[i].size() == 1){

  		}else if(atPrime.trans[i].size() > 1){
  			for(etatset_t alphaTransi : atPrime.trans[i]){
  			/*	if(boucle sur même état){
  					expr = expr + '.' + j + '*';
  				}else{
  					expr = expr + '.' + j;
  				}*/
  			}
  			j = ASCII_A;
  		}
  		
  	}

  	return expr;
  }

////////////////////////////////////////////////////////////////////////////////

  bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2) {

  	//TODO définir cette fonction


  	return false;
  }

////////////////////////////////////////////////////////////////////////////////

  void Help(ostream& out, char *s){
  	out << "Utilisation du programme " << s << " :" << endl ;
  	out << "-acc ou -accept Input Word :\n\t détermine si le mot Word est accepté par l'automate Input" << endl;
  	out << "-det ou -determinize Input Output [-g] :\n\t déterminise l'automate Input, écrit le résultat dans Output" << endl;
  	out << "-isdet ou -is_deterministic Input :\n\t détermine si l'automate Input est déterministe" << endl;
  	out << "-aut2expr ou automate2expressionrationnelle Input :\n\t calcule l'expression rationnelle correspondant à l'automate Input et l'affiche sur la sortie standard" << endl;
  	out << "-expr2aut ou expressionrationnelle2automate ExpressionRationnelle Output [-g] :\n\t calcule l'automate correspondant à ExpressionRationnelle, écrit l'automate résultant dans Output" << endl;
  	out << "-equ ou -equivalent Input1 Intput2 :\n\t détermine si les deux automates Input1 et Input2 sont équivalents" << endl;
  	out << "-nop ou -no_operation Input Output [-g] :\n\t ne fait rien de particulier, recopie l'entrée dans Output" << endl;

  	out << "Exemple '" << s << " -determinize auto.txt resultat -g" << endl;
  }

////////////////////////////////////////////////////////////////////////////////

  int main(int argc, char* argv[] ){
  	if(argc < 3){
  		Help(cout, argv[0]);
  		return EXIT_FAILURE;
  	}

  	int pos;
  int act=-1;                 // pos et act pour savoir quelle action effectuer
  int nb_ifiles = 0;          // nombre de fichiers en entrée
  int nb_ofiles = 0;          // nombre de fichiers en sortie
  string str, in1, in2, out, acc, expr;
  // chaines pour (resp.) tampon; fichier d'entrée Input1; fichier d'entrée Input2;
  // fichier de sortie et chaine dont l'acceptation est à tester 
  bool graphMode=false;     // sortie graphviz ?

  // options acceptées
  const size_t NBOPT = 8;
  string aLN[] = {"accept", "determinize", "is_terministic", "automate2expressionrationnelle", "expressionrationnelle2automate", "equivalent", "no_operation", "graph"};
  string aSN[] = {"acc", "det", "isdet", "aut2expr", "expr2aut", "equ", "nop", "g"};
  
  // on essaie de "parser" chaque option de la ligne de commande
  for(int i=1; i<argc; ++i){
  	if (DEBUG) cerr << "argv[" << i << "] = '" << argv[i] << "'" << endl;
  	str = argv[i];
  	pos = -1;
  	string* pL = find(aLN, aLN+NBOPT, str.substr(1));
  	string* pS = find(aSN, aSN+NBOPT, str.substr(1));

  	if(pL!=aLN+NBOPT)
  		pos = pL - aLN;
  	if(pS!=aSN+NBOPT)
  		pos = pS - aSN;   

  	if(pos != -1){
      // (pos != -1) <=> on a trouvé une option longue ou courte
  		if (DEBUG) cerr << "Key found (" << pos << ") : " << str << endl;
  		switch (pos) {
        case 0: //acc
        in1 = argv[++i];
        acc = argv[++i];
        nb_ifiles = 1;
        nb_ofiles = 0;
        break;
        case 1: //det
        in1 = argv[++i];
        out = argv[++i];
        nb_ifiles = 1;
        nb_ofiles = 1;
        break;
        case 2: //isdet
        in1 = argv[++i];
        nb_ifiles = 1;
        nb_ofiles = 0;
        break;
        case 3: //aut2expr
        in1 = argv[++i];
        nb_ifiles = 1;
        nb_ofiles = 0;
        break;
        case 4: //expr2aut
        expr = argv[++i];
        out = argv[++i];
        nb_ifiles = 0;
        nb_ofiles = 1;
        break;
        case 5: //equ
        in1 = argv[++i];
        in2 = argv[++i];
        nb_ifiles = 2;
        nb_ofiles = 0;
        break;
        case 6: //nop
        in1 = argv[++i];
        out = argv[++i];
        nb_ifiles = 1;
        nb_ofiles = 1;
        break;          
        case 7: //g
        graphMode = true;
        break;
        default:
        return EXIT_FAILURE;
    }
}
else{
	cerr << "Option inconnue "<< str << endl;
	return EXIT_FAILURE;
}

if(pos<7){
	if(act > -1){
		cerr << "Plusieurs actions spécififées"<< endl;
		return EXIT_FAILURE;
	}
	else
		act = pos;
}    
}

if (act == -1){
	cerr << "Pas d'action spécififée"<< endl;
	return EXIT_FAILURE;
}  

/* Les options sont OK, on va essayer de lire le(s) automate(s) at1 (et at2)
et effectuer l'action spécifiée. Atr stockera le résultat*/

sAutoNDE at1, at2, atr;

  // lecture du des fichiers en entrée
if ((nb_ifiles == 1 or nb_ifiles == 2) and !FromFile(at1, in1)){
	cerr << "Erreur de lecture " << in1 << endl;
	return EXIT_FAILURE;
}  
if (nb_ifiles ==2 and !FromFile(at2, in2)){
	cerr << "Erreur de lecture " << in2 << endl;
	return EXIT_FAILURE;
}  

switch(act) {
  case 0: //acc
  if (Accept(at1, acc)){
  	cout << "'" << acc << "' est accepté : OUI\n";
  }
  else {
  	cout << "'" << acc << "' est accepté : NON\n";
  }
  break;
  case 1: //det
  atr = Determinize(at1);
  break;
  case 2: //isdet
  if (EstDeterministe(at1)){
  	cout << "l'automate fourni en entrée est déterministe : OUI\n";
  }
  else {
  	cout << "l'automate fourni en entrée est déterministe : NON\n";
  }
  break;
  case 3: //aut2expr
  expr =  Automate2ExpressionRationnelle(at1);
  cout << "Expression rationnelle résultante :" << endl << expr << endl;
  break;
  case 4: //expr2aut
  atr =  ExpressionRationnelle2Automate(expr);
  break;
  case 5: //equ
  if (Equivalent(at1,at2)){
  	cout << "les deux automates sont équivalents : OUI\n";
  }
  else {
  	cout << "les deux automates sont équivalents : NON\n";
  }
  break;
  case 6: //nop
  atr = at1;
  break;
  default:
  return EXIT_FAILURE;
}

if (nb_ofiles == 1){
    // on affiche le résultat
    // cout << "Automate résultat :\n----------------\n";
    // cout << atr;

    // écriture dans un fichier texte
	ofstream f((out + ".txt").c_str(), ios::trunc); 
	if(f.fail())
		return EXIT_FAILURE;
	f << atr;    

    // génération d'un fichier graphviz
	if(graphMode){
		ToGraph(atr, out + ".gv");
		system(("dot -Tpng " + out + ".gv -o " + out + ".png").c_str());
	}
}

return EXIT_SUCCESS;
}



