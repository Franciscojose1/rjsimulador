<?php

class Grupos {
  private static $dataMinGroups = 1;
  private static $dataMaxGroups = 6;
  private static $infractionsMinGroups = 0;
  private static $infractionsMaxGroups = 2;

  /* @var array $GruposEdad Array con los grupos de edad considerados actualmente. */
  private static $GruposEdad = array(
    1 => array(FilterByInterval::DESDE => 0, FilterByInterval::HASTA => 30),
    2 => array(FilterByInterval::DESDE => 30, FilterByInterval::HASTA => 60),
    3 => array(FilterByInterval::DESDE => 60, FilterByInterval::HASTA => 120)
  );

  /* @var array $GruposExperiencia Array con los grupos de experiencia considerados actualmente. */
  private static $GruposExperiencia = array(
    1 => array(FilterByInterval::DESDE => 0, FilterByInterval::HASTA => 10),
    2 => array(FilterByInterval::DESDE => 10, FilterByInterval::HASTA => 20),
    3 => array(FilterByInterval::DESDE => 20, FilterByInterval::HASTA => 120)
  );

  /* @var array $GruposExperiencia Array con los grupos de experiencia considerados actualmente. */
  private static $GruposKilometrajeMedioAnual = array(
    1 => array(FilterByInterval::DESDE => 0, FilterByInterval::HASTA => 30000),
    2 => array(FilterByInterval::DESDE => 30000, FilterByInterval::HASTA => 50000),
    3 => array(FilterByInterval::DESDE => 50000, FilterByInterval::HASTA => 999999)
  );

  public static function getDataMinGroups() {
    return self::$dataMinGroups;
  }

  public static function getDataMaxGroups() {
    return self::$dataMaxGroups;
  }

  public static function getInfractionsMinGroups() {
    return self::$infractionsMinGroups;
  }

  public static function getInfractionsMaxGroups() {
    return self::$infractionsMaxGroups;
  }

  public static function getGruposEdad() {
    return variable_get('rjsimulador_grupos_edad', self::$GruposEdad);
  }

  public static function getGruposExperiencia() {
    return variable_get('rjsimulador_grupos_experiencia', self::$GruposExperiencia);
  }

  public static function getGruposKmMedioAnual() {
    return variable_get('rjsimulador_grupos_kilometraje', self::$GruposKilometrajeMedioAnual);
  }
}