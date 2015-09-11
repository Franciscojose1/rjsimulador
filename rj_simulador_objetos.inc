<?php
class PartidaObject {
	public $id_partida;
	public $uid;
	public $fecha;
	public $id_simulacion;
	public $nombre_simulacion;
	public $infracciones;	// Array de objetos Infraccion
	public $datos;			// Array de objetos DatoInstantaneo

	/*
	  Contructor Partida
	*/
	public function __construct(){
      $argv = func_get_args();
      switch( func_num_args() ) {
        case 0:
          self::__construct0();
          break;
        case 1:
          self::__construct1($argv[0]);
          break;
        case 2:
          self::__construct2( $argv[0], $argv[1] );
          break;
        }
	}
	
    public function __construct0(){
    }
	
	/*
	  Contructor -> Recupera los datos de una partida de la BD
	  @param int id_encuesta -> ID de la encuesta a recuperar
	*/
	public function __construct1($id_partida){
      $query = db_select('rjsim_partida', 'p');
	  $query->join('rjsim_simulacion', 's', 'p.id_simulacion = s.id_simulacion');
	  $query->fields('p',array('uid','fecha', 'id_simulacion'))
	        ->fields('s',array('nombre_simulacion'))
			->condition('id_partida',$id_partida,'=');
	  $resultado = $query -> execute();
	  while($record = $resultado -> fetchAssoc()){
	    $this->id_partida = $id_partida;
		$this->uid = $record['uid'];
        $this->fecha = $record['fecha'];
		$this->id_simulacion = $record['id_simulacion'];
        $this->nombre_simulacion = $record['nombre_simulacion'];
		$this->infracciones = array();
		$this->datos = array();
	  }
	}

    public function __construct2($id_partida,$all){
      // Empty
    }

	public function insertInDatabase(){
        $id_partida = db_insert('rjsim_partida')
            ->fields(array(
			    'uid' => $this->uid,
                'fecha' => $this->fecha,
                'id_simulacion' => $this->id_simulacion,
            ))
            ->execute();
        $this->id_partida = $id_partida;

        foreach ($this->infracciones as $infraccion){
          //$infraccion->insertInDatabase();
        }
		
		foreach ($this->datos as $dato){
		  //$dato->insertInDatabase();
		}
    }
	
	public function retrieveDatos() {
		
	}
	
	public function retrieveInfracciones(){
		
	}
}