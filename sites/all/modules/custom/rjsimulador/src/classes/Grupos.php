<?php

class Grupos {
  /* @var array $GruposEdad Array con los grupos de edad considerados actualmente. */
  private static $GruposEdad = array(
    1 => array("desde" => 0, "hasta" => 30),
    2 => array("desde" => 30, "hasta" => 60),
    3 => array("desde" => 60, "hasta" => 120)
  );

  /* @var array $GruposExperiencia Array con los grupos de experiencia considerados actualmente. */
  private static $GruposExperiencia = array(
    1 => array("desde" => 0, "hasta" => 10),
    2 => array("desde" => 10, "hasta" => 20),
    3 => array("desde" => 20, "hasta" => 120)
  );

  /* @var array $GruposExperiencia Array con los grupos de experiencia considerados actualmente. */
  private static $GruposKilometrajeMedioAnual = array(
    1 => array("desde" => 0, "hasta" => 30000),
    2 => array("desde" => 30000, "hasta" => 50000),
    3 => array("desde" => 50000, "hasta" => 999999)
  );

  public static function getGruposEdad() {
    return self::$GruposEdad;
  }

  public static function getGruposExperiencia() {
    return self::$GruposExperiencia;
  }

  public static function getGrupoKmMedioAnual() {
    return self::$GruposKilometrajeMedioAnual;
  }
}