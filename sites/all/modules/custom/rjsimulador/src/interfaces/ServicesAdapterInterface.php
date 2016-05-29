<?php

/**
 * Interface ServicesAdapterInterface Interfaz que deben implementar los objetos que vayan a mandarse por WebService.
 */
interface ServicesAdapterInterface {
  /**
   * Permite recuperar un objeto como un array para ser parseado de forma simple a JSON.
   * @return array Array asociativo con las propiedades del objeto.
   */
  public function convertPropertiesToArray();
} 