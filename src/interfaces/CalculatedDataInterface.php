<?php

interface CalculatedDataInterface {
  /**
   * @param Lista $lista
   * @return mixed Devuelve el cálculo del campo.
   */
  public function calculate(Lista $lista);
} 