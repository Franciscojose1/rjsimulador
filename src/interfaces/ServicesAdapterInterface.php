<?php

interface ServicesAdapterInterface
{
  /**
   * Permite recuperar un objeto como un array para ser parseado de forma simple a JSON.
   * @return array Array asociativo con las propiedades del objeto.
   */
  public function convertPropertiesToArray();
} 