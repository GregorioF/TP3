#ifndef DICCT_H
#define DICCT_H

#include "pila.h"
#include "Conj.h"
#include <iostream>


using namespace aed2;
using namespace std;

template < typename T>
class dicT
{
public:
	class Iterador;
	///////////////////////////
	dicT();  //definida
	///////////////////////////
	dicT(const dicT<T> &otro);//definida
	///////////////////////////
	~dicT();  //definida
	///////////////////////////
	bool esVacio() const;  //definida
	///////////////////////////
	void definir(string clave, T& elem);  //definida
	///////////////////////////
	bool definido(string clave) const;  //definida
	///////////////////////////
	T& obtener(string clave) const;  //definida
	///////////////////////////
	void borrar(string clave);  //definida
	///////////////////////////
	aed2::Conj<string> claves();  //definida
	///////////////////////////
	Iterador CrearIt();  //definida
	///////////////////////////
	bool operator ==(const dicT<T> &otro) const;  //definida
	///////////////////////////
	string Minimo(); //definida
	///////////////////////////
	string Maximo(); //definida
	///////////////////////////


	class Iterador{
	public:
		Iterador();
        ///////////////////////////
		Iterador(const typename dicT<T>::Iterador& otra);
		///////////////////////////
		bool HaySiguiente() const;
		///////////////////////////
		string  SiguienteClave();
		///////////////////////////
		T& SiguienteSignificado();
		///////////////////////////
		void Avanzar();
	private:

		pair<string, T> Siguiente;
		aed2::Conj<string> claves;
		dicT<T>* dic;
	};

private:
	struct Nodo{
		Nodo():  letra('a'), padre(NULL){
			ponerTodoEnNull(hijos);
		}
		Nodo(char c, Nodo* p) :  letra(c), padre(p){
			ponerTodoEnNull(hijos);
		} 
		Nodo(char c, Nodo* p, T& elem ): significado(&elem), letra(c), padre(p){
			ponerTodoEnNull(hijos);
		} 
		T* significado;
		char letra;
		Nodo* padre;
		Nodo* hijos [256];
		

		void ponerTodoEnNull(Nodo* hijos []){
			
			for(int i=0; i<256; i++) hijos[i]=NULL;
		}
	};

	Nodo* raiz;
	////////////////////////////////////////////////////////////////////////////////
	//FUNCIONES AUXILIARES PRIVADAS
	////////////////////////////////////////////////////////////////////////////////
	
	// Se utiliza en Claves()
	template <typename K>
	void apilarHijos(Nodo* actual, pila<K>& recoridos){
		for(int i=255;i>=0;i--){
			if(actual->hijos[i]!=NULL)recoridos.apilar(*actual->hijos[i]);
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	//No se utiliza por ahora...
	Lista<Nodo*> hijos(Nodo* actual){
		Lista<Nodo*> res;
		int i=0;
		while(i<256){
			if(actual->hijos[i]!=NULL)res.AgregarAtras(actual->hijos[i]);
			i++;
		}
		return res;
	}
	////////////////////////////////////////////////////////////////////////////////
	//Se utiliza en borrar() y en Claves() devuelve true si no tiene hijos el nodo Actual
	bool EsVaciaHijos(Nodo* actual){
		bool res=true;
		int i=0;
		while(i<256){
			if(actual->hijos[i]!=NULL) return false;
			i++;
		}
		return true;}
	////////////////////////////////////////////////////////////////////////////////
	//se utiliza en claves(), en Minimo(), y en Maximo() devuelve si el puntero a significado es != Null
	bool esPalabra(Nodo* actual){
		return (actual->significado!=NULL);
	}
	////////////////////////////////////////////////////////////////////////////////
	//Se utiliza en Claves(), en Minimo(), y en Maximo() Devuelve la palabra que se forma hasta n
	string DameNombre(Nodo* n){
		Nodo* actual= n;
		string res;
		while(actual->padre!=NULL){
			string aux;
			aux.push_back(actual->letra);
			res.insert(0,aux);
			actual=actual->padre;
		}
		return res;
	}
	////////////////////////////////////////////////////////////////////////////////
	//Se utiliza en el destructor, desturye todos los nodos desde actual, asi abajo
	void destruir(Nodo* actual){
		if(actual!=NULL){
			for(int i=0; i<255;i++) destruir(actual->hijos[i]);
			delete(actual);
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	//Se utiliza en  Minimo() devuelve si tiene un Hijo menor que  n
	bool tieneUnHijosMasChico(Nodo* n){
		int i=0;
		Nodo* actual=NULL;
		while(actual==NULL && i<256){
			actual=n->hijos[i];
			i++;
		}
		if(actual==NULL) return false;
		else if(actual->letra < n->letra)return true;
		else return false;
	}	
	////////////////////////////////////////////////////////////////////////////////
	// Se utiliza en Maximo()
	bool tieneUnHijosMasGrande(Nodo* n){
		int i=255;
		Nodo* actual=NULL;
		while(actual==NULL && i>=0){
			actual=n->hijos[i];
			i--;
		}
		if(actual==NULL) return false;
		else if(actual->letra < n->letra)return true;
		else return false;
	}	
};
///////////////////////////////////////////////////////////
//ALGORITMOS FUNCIONES INTERFAZ
///////////////////////////////////////////////////////////
template <typename T>
dicT<T>::dicT(){
	raiz= new Nodo();
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
template <typename T>
dicT<T>::~dicT(){
	destruir(raiz);
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
template <typename T>
bool dicT<T>::esVacio()const {
	int i=0;
	while(i<27){
		if(raiz->hijos[i]!=NULL) return false;
		i++;
	}
	return true;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
template<typename T>
void dicT<T>::definir(string clave, T& elem){
	Nodo* actual = raiz;
	int i=0;
	int tamanio = clave.size();
	while(i<tamanio){
		if( actual->hijos[clave[i]] != NULL) actual= actual->hijos[clave[i]];
		else {
			actual->hijos[clave[i]] = new Nodo( clave[i], actual);
			actual= actual->hijos[clave[i]];
		}
		i++;
	}	
	actual->significado= &elem;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
template <typename T>
bool dicT<T>::definido(string clave) const{
	Nodo* actual = raiz;
	int i=0;
	while(actual->hijos[clave[i]] != NULL){
		actual = actual->hijos[clave[i]];
		i++;
	}
	if(i==clave.size()){
		return true;
	}
	else return false;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
template<typename T>
T& dicT<T>::obtener(string clave) const{
	Nodo* actual = raiz;
	int i=0;
	int tamanio = clave.size();
	while(i<tamanio){
		actual= actual->hijos[clave[i]];
		i++;
	}	
	return *(actual->significado);
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
template<typename T>
void dicT<T>::borrar(string s){
	Nodo* actual= raiz;
	int i=0;
	int tamanio=s.size();
	pila<Nodo> recorridos;
	while(i<tamanio){
		recorridos.apilar(*actual);
		actual= actual -> hijos[s[i]];
		i++;
	}
	recorridos.apilar(*actual);
	while(recorridos.size()>1){
		actual=&recorridos.tope();
		recorridos.desapilar();
		if(EsVaciaHijos(actual)){
			int aux= actual->letra;
			recorridos.tope().hijos[aux]=0;
			delete(actual);
		}
	}
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
template<typename T>
aed2::Conj<string> dicT<T>::claves() {
	Nodo* actual= raiz;
	pila<Nodo> recorridos;
	apilarHijos(actual, recorridos);
	Conj<string> res;
	int contador=0;
	while(!(recorridos.EsVacia())){
		actual = &recorridos.tope();
		recorridos.desapilar();
		if(! EsVaciaHijos(actual)) apilarHijos(actual, recorridos);
		if(esPalabra(actual)){
			string aux= DameNombre(actual); ///ANDA DAME NOMBRE ! :D
			res.AgregarRapido(aux); 	
		}
	}	
	return res;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
template <typename T>
bool dicT<T>::operator ==(const dicT<T> &otro) const{
	bool res;
	typename Conj<string>::Iterador it = otro.claves().CrearIt();
	while(it.HaySiguiente()){
		if(!definido(it.Siguiente())) return false;
		if(otro.obtener(it.Siguiente()) != this.obtener(it.Siguiente())) return false;
		it.Avanzar();
	}
	return true;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
template <typename T>
string dicT<T>::Minimo(){
	Nodo* actual=NULL; // para q entre en el primer while
	int i=0;
	while(actual==NULL){
		actual = raiz->hijos[i];
		i++;
	}
	while(tieneUnHijosMasChico(actual) || !esPalabra(actual)){
		Nodo* aux=NULL;
		i=0;
		while(aux==NULL){
			aux=actual->hijos[i];
			i++;
		}
		actual=aux;
	}
	return DameNombre(actual);
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
template <typename T>
string dicT<T>::Maximo(){
	Nodo* actual=NULL; // para q entre en el primer while
	int i=255;
	while(actual==NULL){
		actual = raiz->hijos[i];
		i--;
	}
	while(tieneUnHijosMasGrande(actual) || !esPalabra(actual)){
		Nodo* aux=NULL;
		i=255;
		while(aux==NULL){
			aux=actual->hijos[i];
			i--;
		}
		actual=aux;
	}
	return DameNombre(actual);
}


#endif