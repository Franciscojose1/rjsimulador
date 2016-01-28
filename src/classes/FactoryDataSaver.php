<?php

class FactoryDataSaver
{
  const DATABASE = 'database';

  /**
   * @param string $config Un valor constante de FactoryDataSaver para seleccionar la instancia
   * @return DataSaver
   * @throws Exception Si no se selecciona un tipo adecuado en la Factory.
   */
  public static function createDataSaver($config = self::DATABASE)
  {
    switch ($config) {
      case self::DATABASE:
        return DBDataSaver::getInstance();
      default:
        throw new Exception("Error recuperando el DataSaver.");
    }
  }
} 