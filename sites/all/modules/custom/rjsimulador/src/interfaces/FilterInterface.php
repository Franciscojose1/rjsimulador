<?php

/**
 * Interface FilterInterface Interfaz que deben implementar las clases para filtrar los objetos de una lista.
 */
interface FilterInterface {
  /**
   * Filtra un item de una lista según las condiciones establecidas.
   *
   * Permite filtrar una Lista según unas condiciones establecidas. Las condiciones deben ser establecidas en la clase implementadora
   * de la interfaz.
   *
   * @param mixed $item
   * @return bool Devuelve si el item cumple con los criterios del filtro.
   * @throws Exception Si el elemento es de un tipo no soportado por el filtro.
   */
  public function filter($item);
} 