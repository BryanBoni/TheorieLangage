/*


*/
/*
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

string Automate2ExpressionRationnelle(sAutoNDE at){
  cout << "Construction d'une expression rationnelle à partir d'un automate\n";

  string sr;

  //TODO définir cette fonction

  return sr;
}

////////////////////////////////////////////////////////////////////////////////

bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2) {

  //TODO définir cette fonction

  return false;
}

////////////////////////////////////////////////////////////////////////////////
*/


//Provisoir le temps de créer les fonction
/*
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
/*
  
return EXIT_SUCCESS;
}
*/