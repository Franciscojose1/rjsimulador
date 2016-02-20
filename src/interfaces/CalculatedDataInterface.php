<?php

interface CalculatedDataInterface {
  /**
   * @param Lista $lista
   * @return mixed Devuelve el cálculo del campo de los elementos de la lista pasada.
   * @throws Exception Si el campo pasado no es admisible para el tipo de lista o si la lista no es un tipo soportado.
   */
  public function calculate(Lista $lista);
} 