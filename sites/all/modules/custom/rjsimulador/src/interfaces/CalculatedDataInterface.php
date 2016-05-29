<?php

/**
 * Interface CalculatedDataInterface Interfaz que deben implementar las clases para hacer cálculos sobre los objetos de una lista.
 */
interface CalculatedDataInterface {
  /**
   * Realiza el cálculo adecuado según los datos de entrada para la clase implementadora de esta interfaz
   *
   * @param Lista $lista
   * @return mixed Devuelve el cálculo del campo de los elementos de la lista pasada.
   * @throws Exception Si el campo pasado no es admisible para el tipo de lista o si la lista no es un tipo soportado.
   */
  public function calculate(Lista $lista);
} 