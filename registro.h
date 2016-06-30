#ifndef REGISTRO_H
#define REGISTRO_H

#include "Conj.h"
#include "dato.h"

using namespace aed2;


class Registro{
public:
		class Iterador;
    class const_Iterador;

    Registro();
    Registro(const Registro& otro);
    Registro& operator=(const Registro& otro);

    Iterador Definir(const NombreCampo& clave, const dato& significado);
    Iterador DefinirRapido(const NombreCampo& clave, const dato& significado);

    bool Definido(const NombreCampo& clave) const;
    const dato& Significado(const NombreCampo& clave) const;
    dato& Significado(const NombreCampo& clave);
    void Borrar(const NombreCampo& clave);
    Nat CantClaves() const;
    Iterador CrearIt();
    const_Iterador CrearIt() const;

    //Estas funciones son utiles para saber si algo esta definido
    //y ver cual es su signficado, sin recorrer dos veces.
    Iterador Buscar(const NombreCampo&);
    const_Iterador Buscar(const NombreCampo&) const;

    class Iterador
    {
      public:

        Iterador();

        Iterador(const typename Registro::Iterador& otro);

        Iterador& operator = (const typename Registro::Iterador& otro);

        bool operator == (const typename Registro::Iterador&) const;

        bool HaySiguiente() const;
        bool HayAnterior() const;
        const NombreCampo& SiguienteClave() const;
        dato& SiguienteSignificado();
        const NombreCampo& AnteriorClave() const;
        dato& AnteriorSignificado();
        Elem Siguiente();
        Elem Anterior();
        void Avanzar();
        void Retroceder();
        void EliminarSiguiente();
        void EliminarAnterior();

      private:

        typename Lista<NombreCampo>::Iterador it_claves_;
        typename Lista<S>::Iterador it_significados_;

        Iterador(Registro* d);

        friend typename Registro::Iterador Registro::CrearIt();
        friend class Registro::const_Iterador;
    };

    class const_Iterador
    {
      public:

        const_Iterador();

        const_Iterador(const typename Registro::Iterador& otro);

        const_Iterador(const typename Registro::const_Iterador& otro);

        const_Iterador& operator = (const typename Registro::const_Iterador& otro);

        bool operator==(const typename Registro::const_Iterador&) const;

        bool HaySiguiente() const;
        bool HayAnterior() const;
        const NombreCampo& SiguienteClave() const;
        const dato& SiguienteSignificado() const;
        const NombreCampo& AnteriorClave() const;
        const dato& AnteriorSignificado() const;
        const_Elem Siguiente() const;
        const_Elem Anterior() const;
        void Avanzar();
        void Retroceder();

    private:

        typename Lista<NombreCampo>::const_Iterador it_claves_;
        typename Lista<dato>::const_Iterador it_significados_;

        const_Iterador(const Registro* d);

        friend typename Registro::const_Iterador Registro::CrearIt() const;
    };

    struct Elem
    {
      public:

        const NombreCampo& clave;
        dato& significado;

        Elem(const NombreCampo& c, dato& s) : clave(c), significado(s) {}
        //Para sacar esto de aca, necesitariamos definir rasgos y otras yerbas
        //Lamentablemente, sino C++ no reconoce bien los tipos

        friend std::ostream& operator<<(std::ostream& os, const Registro::Elem& e) {
          return os << e.clave << ":" << e.significado;
        }

      private:

        typename Registro::Elem& operator=(const Registro::Elem&);
    };

    struct const_Elem
    {
      public:

        const NombreCampo& clave;
        const dato& significado;

        const_Elem(const NombreCampo& c, const dato& s) : clave(c), significado(s) {}

        //Para sacar esto de aca, necesitariamos definir rasgos y otras yerbas
        //Lamentablemente, sino C++ no reconoce bien los tipos
        friend std::ostream& operator << (std::ostream& os, const Registro::const_Elem& e) {
          return os << e.clave << ":" << e.significado;
        }

      private:

        typename Registro::const_Elem& operator = (const Registro::const_Elem&);
    };

  private:

    Dicc<NombreCampo, dato> dic;

};


std::ostream& operator << (std::ostream &os, const Dicc<NombreCampo,S>& d);


bool operator == (const Dicc<K,S>& d1, const Dicc<K,S>& d2);

  // Implementacion Dicc


Dicc<K,S>::Dicc()
{}


Dicc<K,S>::Dicc(const Dicc<K,S>& otro)
  : claves_(otro.claves_), significados_(otro.significados_)
{}


Dicc<K,S>& Dicc<K,S>::operator = (const Dicc<K,S>& otro)
{
  claves_ = otro.claves_;
  significados_ = otro.significados_;

  return *this;
}


typename Dicc<K,S>::Iterador Dicc<K,S>::Definir(const NombreCampo& clave, const dato& significado)
{
  Iterador it = Buscar(clave);

  if(it.HaySiguiente()) {
    it.SiguienteSignificado() = significado;
  } else {
    it = DefinirRapido(clave, significado);
  }

  return it;
}


typename Dicc<K,S>::Iterador Dicc<K,S>::DefinirRapido(const K& clave, const S& significado)
{
  #ifdef DEBUG
  assert( not Definido(clave) );
  #endif

  claves_.AgregarAdelante(clave);
  significados_.AgregarAdelante(significado);

  return CrearIt();
}


bool Dicc<K,S>::Definido(const K& clave) const
{
  return Buscar(clave).HaySiguiente();
}


const S& Dicc<K,S>::Significado(const K& clave)const
{
  #ifdef DEBUG
  assert( Definido(clave) );
  #endif

  return Buscar(clave).SiguienteSignificado();
}

template<class K, class S>
S& Dicc<K,S>::Significado(const K& clave)
{
  #ifdef DEBUG
  assert( Definido(clave) );
  #endif

  return Buscar(clave).SiguienteSignificado();
}

template<class K, class S>
void Dicc<K,S>::Borrar(const K& clave)
{
  #ifdef DEBUG
  assert( Definido(clave) );
  #endif

  Buscar(clave).EliminarSiguiente();
}

template<class K, class S>
Nat Dicc<K,S>::CantClaves() const
{
  return claves_.Longitud();
}

template<class K, class S>
typename Dicc<K,S>::Iterador Dicc<K,S>::CrearIt()
{
  return Iterador(this);
}

template<class K, class S>
typename Dicc<K,S>::const_Iterador Dicc<K,S>::CrearIt() const
{
  return const_Iterador(this);
}

  // Implementacion Iterador

template<class K, class S>
Dicc<K,S>::Iterador::Iterador()
{}

template<class K, class S>
Dicc<K,S>::Iterador::Iterador(const typename Dicc<K, S>::Iterador& otro)
  : it_claves_(otro.it_claves_), it_significados_(otro.it_significados_)
{}

template<class K, class S>
typename Dicc<K,S>::Iterador& Dicc<K,S>::Iterador::operator = (const typename Dicc<K, S>::Iterador& otro)
{
  it_claves_ = otro.it_claves_;
  it_significados_ = otro.it_significados_;

  return *this;
}

template<class K, class S>
bool Dicc<K,S>::Iterador::HaySiguiente() const
{
  return it_claves_.HaySiguiente();
}

template<class K, class S>
bool Dicc<K,S>::Iterador::HayAnterior() const
{
  return it_claves_.HayAnterior();
}

template<class K, class S>
const K& Dicc<K,S>::Iterador::SiguienteClave() const
{
  #ifdef DEBUG
  assert(HaySiguiente());
  #endif

  return it_claves_.Siguiente();
}

template<class K, class S>
S& Dicc<K,S>::Iterador::SiguienteSignificado()
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  return it_significados_.Siguiente();
}

template<class K, class S>
typename Dicc<K,S>::Elem Dicc<K,S>::Iterador::Siguiente()
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  return Elem(SiguienteClave(), SiguienteSignificado());
}

template<class K, class S>
const K& Dicc<K,S>::Iterador::AnteriorClave() const
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  return it_claves_.Anterior();
}

template<class K, class S>
S& Dicc<K,S>::Iterador::AnteriorSignificado()
{
  #ifdef DEBUG
  assert(HayAnterior());
  #endif

  return it_significados_.Anterior();
}

template<class K, class S>
typename Dicc<K,S>::Elem Dicc<K,S>::Iterador::Anterior()
{
  #ifdef DEBUG
  assert(HayAnterior());
  #endif

  return Elem(AnteriorClave(), AnteriorSignificado());
}

template<class K, class S>
void Dicc<K,S>::Iterador::Avanzar()
{
  #ifdef DEBUG
  assert(HaySiguiente());
  #endif

  it_claves_.Avanzar();
  it_significados_.Avanzar();
}

template<class K, class S>
void Dicc<K,S>::Iterador::Retroceder()
{
  #ifdef DEBUG
  assert(HayAnterior());
  #endif

  it_claves_.Retroceder();
  it_significados_.Retroceder();
}

template<class K, class S>
Dicc<K,S>::Iterador::Iterador(Dicc<K,S>* d)
  : it_claves_(d->claves_.CrearIt()), it_significados_(d->significados_.CrearIt())
{}

template<class K, class S>
void Dicc<K,S>::Iterador::EliminarSiguiente()
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  it_claves_.EliminarSiguiente();
  it_significados_.EliminarSiguiente();
}

template<class K, class S>
void Dicc<K,S>::Iterador::EliminarAnterior()
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  it_claves_.EliminarAnterior();
  it_significados_.EliminarAnterior();
}

  // Implementacion const_Iterador

template<class K, class S>
Dicc<K,S>::const_Iterador::const_Iterador()
{}

template<class K, class S>
Dicc<K,S>::const_Iterador::const_Iterador(const typename Dicc<K,S>::Iterador& otro)
  : it_claves_(otro.it_claves_), it_significados_(otro.it_significados_)
{}

template<class K, class S>
Dicc<K,S>::const_Iterador::const_Iterador(const typename Dicc<K, S>::const_Iterador& otro)
  : it_claves_(otro.it_claves_), it_significados_(otro.it_significados_)
{}

template<class K, class S>
typename Dicc<K,S>::const_Iterador& Dicc<K,S>::const_Iterador::operator=(const typename Dicc<K, S>::const_Iterador& otro)
{
  it_claves_ = otro.it_claves_;
  it_significados_ = otro.it_significados_;

  return *this;
}

template<class K, class S>
bool Dicc<K,S>::const_Iterador::HaySiguiente() const
{
  return it_claves_.HaySiguiente();
}

template<class K, class S>
bool Dicc<K,S>::const_Iterador::HayAnterior() const
{
  return it_claves_.HayAnterior();
}

template<class K, class S>
const K& Dicc<K,S>::const_Iterador::SiguienteClave() const
{
  #ifdef DEBUG
  assert(HaySiguiente());
  #endif

  return it_claves_.Siguiente();
}

template<class K, class S>
const S& Dicc<K,S>::const_Iterador::SiguienteSignificado() const
{
  #ifdef DEBUG
  assert(HaySiguiente());
  #endif

  return it_significados_.Siguiente();
}

template<class K, class S>
typename Dicc<K,S>::const_Elem Dicc<K,S>::const_Iterador::Siguiente() const
{
  #ifdef DEBUG
  assert( HaySiguiente() );
  #endif

  return const_Elem(SiguienteClave(), SiguienteSignificado());
}

template<class K, class S>
const K& Dicc<K,S>::const_Iterador::AnteriorClave() const
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  return it_claves_.Anterior();
}

template<class K, class S>
const S& Dicc<K,S>::const_Iterador::AnteriorSignificado() const
{
  #ifdef DEBUG
  assert(HayAnterior());
  #endif

  return it_significados_.Anterior();
}

template<class K, class S>
typename Dicc<K,S>::const_Elem Dicc<K,S>::const_Iterador::Anterior() const
{
  #ifdef DEBUG
  assert( HayAnterior() );
  #endif

  return const_Elem(AnteriorClave(), AnteriorSignificado());
}

template<class K, class S>
void Dicc<K,S>::const_Iterador::Avanzar()
{
  #ifdef DEBUG
  assert(HaySiguiente());
  #endif

  it_claves_.Avanzar();
  it_significados_.Avanzar();
}

template<class K, class S>
void Dicc<K,S>::const_Iterador::Retroceder()
{
  #ifdef DEBUG
  assert(HayAnterior());
  #endif

  it_claves_.Retroceder();
  it_significados_.Retroceder();
}

template<class K, class S>
Dicc<K,S>::const_Iterador::const_Iterador(const Dicc<K,S>* d)
  : it_claves_(d->claves_.CrearIt()), it_significados_(d->significados_.CrearIt())
{}

template<class K, class S>
bool Dicc<K,S>::const_Iterador::operator == (const typename Dicc<K,S>::const_Iterador& otro) const
{
  return it_claves_ == otro.it_claves_ && it_significados_ == otro.it_significados_;
}

template<class K, class S>
bool Dicc<K,S>::Iterador::operator == (const typename Dicc<K,S>::Iterador& otro) const
{
  return it_claves_ == otro.it_claves_ && it_significados_ == otro.it_significados_;
}

  ///Funciones auxiliares

template<class K, class S>
typename Dicc<K,S>::Iterador Dicc<K,S>::Buscar(const K& clave)
{
  typename Dicc<K,S>::Iterador it = CrearIt();
  while(it.HaySiguiente() && !(it.SiguienteClave() == clave)){
    it.Avanzar();
  }
  return it;
}

template<class K, class S>
typename Dicc<K,S>::const_Iterador Dicc<K,S>::Buscar(const K& clave) const
{
  typename Dicc<K,S>::const_Iterador it = CrearIt();
  while(it.HaySiguiente() && !(it.SiguienteClave() == clave)){
      it.Avanzar();
  }
  return it;
}

template<class K, class S>
std::ostream& operator << (std::ostream& os, const Dicc<K,S>& d)
{
  return Mostrar(os, d, '{', '}');
}

template<class K, class S>
bool operator == (const Dicc<K,S>& d1, const Dicc<K,S>& d2)
{
  bool retval = d1.CantClaves() == d2.CantClaves();
  typename Dicc<K,S>::const_Iterador it1 = d1.CrearIt();
  while(retval and it1.HaySiguiente()){
    typename Dicc<K,S>::const_Iterador it2 = d2.Buscar(it1.SiguienteClave());
    retval = it2.HaySiguiente() and it1.SiguienteSignificado() == it2.SiguienteSignificado();
    it1.Avanzar();
  }
  return retval;
}

}






void mergear(Registro& r1, Registro& r2){
	typename Dicc<NombreCampo, Dato>::Iterador itR2= r2.CrearIt();

	while(itR2.HaySiguiente()){
		if(!(r1.Definido(itR2.SiguienteClave()))) 
			r1.Definir(itR2.SiguienteClave(), itR2.SiguienteSignificado());
		itR2.Avanzar();
	}	
}

Conj<NombreCampo> campos(Registro& r1){
	Conj<NombreCampo> res;
	typename Dicc<NombreCampo, Dato>::Iterador itR1= r1.CrearIt();
	while(itR1.HaySiguiente()){
		res.AgregarRapido(itR1.SiguienteClave());
		itR1.Avanzar();
	}
	return res;
}

bool coincidenTodos(Registro& r1, Conj<NombreCampo>& c, Registro& r2){
	bool res=true;
	if(!c.EsVacio()){
		typename::Conj<NombreCampo>::Iterador it=c.CrearIt();
		while(it.HaySiguiente() && res==true){
			if(r1.Significado(it.Siguiente()).tipo() != r2.Significado(it.Siguiente()).tipo()){
				res=false;
				}
			else{
				if(r1.Significado(it.Siguiente()).esNat()){
					if(r1.Significado(it.Siguiente()).dameNat() != r2.Significado(it.Siguiente()).dameNat()){res=false;}
					}
				else{
					if(r1.Significado(it.Siguiente()).dameString() != r2.Significado(it.Siguiente()).dameString()){res=false;}
					}	
				}
		
			it.Avanzar();	
			}		
		}
	return res;	
}

bool borrar(Registro crit, Registro reg){
	bool res=true;
	Conj<NombreCampo> c=campos(crit);
	res=coincidenTodos(crit, c, reg);
	return res;
}
	

void copiarCampos(Conj<NombreCampo> c, Registro& r1, Registro r2){
	typename::Conj<NombreCampo>::Iterador it=c.CrearIt();
	while(it.HaySiguiente()){
		if(!r1.Definido(it.Siguiente())){
			r1.Definir(it.Siguiente(), r2.Significado(it.Siguiente()));
			}
		it.Avanzar();	
		} 
	}
	
#endif
