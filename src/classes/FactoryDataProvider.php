<?php

class FactoryDataProvider
{
  const DATABASE = 'database';

  /**
   * @param string $config Un valor constante de FactoryDataProvider para seleccionar instancia
   * @return DataProvider
   * @throws Exception Si no se selecciona un tipo adecuado en la Factory.
   */
  public static function createDataProvider($config = self::DATABASE)
  {
    switch ($config) {
      case self::DATABASE:
        return DBDataProvider::getInstance();
      default:
        throw new Exception("Error recuperando el DataProvider.");
    }
  }
} 