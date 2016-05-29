<?php

/**
 * Class FactoryDataManager Factoría para crear las clases adecuadas de proveedores de datos.
 * Por defecto recupera los datos de la base de datos.
 */
class FactoryDataManager {
  const DATABASE = 'database';

  /**
   * @param string $config Un valor constante de FactoryDataManager para seleccionar instancia
   * @return DataProvider
   * @throws Exception Si no se selecciona un tipo adecuado en la Factory.
   */
  public static function createDataProvider($config = self::DATABASE) {
    switch ($config) {
      case self::DATABASE:
        return DBDataProvider::getInstance();
        break;
      default:
        throw new Exception("Error recuperando el DataProvider.");
        break;
    }
  }

  /**
   * @param string $config Un valor constante de FactoryDataManager para seleccionar la instancia
   * @return DataSaver
   * @throws Exception Si no se selecciona un tipo adecuado en la Factory.
   */
  public static function createDataSaver($config = self::DATABASE) {
    switch ($config) {
      case self::DATABASE:
        return DBDataSaver::getInstance();
        break;
      default:
        throw new Exception("Error recuperando el DataSaver.");
        break;
    }
  }

  /**
   * @param string $config Un valor constante de FactoryDataManager para seleccionar la instancia
   * @return DataRemover
   * @throws Exception Si no se selecciona un tipo adecuado en la Factory.
   */
  public static function createDataRemover($config = self::DATABASE) {
    switch ($config) {
      case self::DATABASE:
        return DBDataRemover::getInstance();
        break;
      default:
        throw new Exception("Error recuperando el DataRemover.");
        break;
    }
  }
} 