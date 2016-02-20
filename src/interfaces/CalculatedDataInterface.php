<?php

interface CalculatedDataInterface {
  /**
   * @param Lista $lista
   * @return mixed Devuelve el cálculo del campo de los elementos de la lista pasada.
   */
  public function calculate(Lista $lista);
} 