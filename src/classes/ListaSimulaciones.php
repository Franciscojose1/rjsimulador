<?php

class ListaSimulaciones extends Lista {
  /**
   * @return Simulacion
   */
  public function current() {
    return parent::current();
  }

  /**
   * @param int $numberKey Clave del elemento a recuperar.
   * @return Simulacion Devuelve el item de la lista en esa posición.
   * @throws InvalidArgumentException Si la key pasada no es numérica.
   * @throws Exception Si no existe esa clave en la lista.
   */
  public function get($numberKey) {
    return parent::get($numberKey);
  }

  /**
   * @param Simulacion $item Elemento a introducir en la lista.
   * @return int Número de elementos en la lista después de añadir la simulación.
   * @throws InvalidArgumentException Si el item pasado no es de tipo Simulacion.
   */
  public function add($item) {
    if ($item instanceof Simulacion) {
      parent::add($item);
    }
    else {
      throw new InvalidArgumentException("El item a añadir no es de tipo Simulación.");
    }

    return $this->count();
  }

  /**
   * @param int $numberKey Clave del elemento al eliminar.
   * @return int Número de elementos en la lista después de eliminar la simulación.
   * @throws InvalidArgumentException Si la clave no es numérica.
   * @throws Exception Si no existe esa clave en la lista.
   */
  public function remove($numberKey) {
    return parent::remove($numberKey);
  }
}