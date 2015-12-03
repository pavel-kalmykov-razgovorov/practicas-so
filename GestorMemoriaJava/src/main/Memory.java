package main;

import java.util.ArrayList;

public class Memory {
	public static final int MAXMEMORIA = 2000;
	private ArrayList<Process> processes = new ArrayList<Process>();
	
	public Memory() {
		//Tratamos el hueco restante de toda la memoria como si fuera un proceso
		processes.add(new Process());
	}

	public boolean isEmpty() { return processes.size() == 1; } //Estará vacía si sólo está el proceso base
	
	@Override
	public String toString() { //Vamos concatenando todos los procesos que tenga la memoria
		StringBuilder sb = new StringBuilder();
		for(Process p : processes) sb.append(p.toString() + " ");
		return sb.toString();
	}
	
	//Devolvemos un string de 20 caracteres que nos servirá de mapa para
	//pintar cada bloque de memoria de un color en función del número que tenga
	//el proceso dentro del campo nombre. Si el nombre del proceso 
	//no lleva ningún número, se verá como si fuera hueco
	public String toMap() {
		StringBuilder sb = new StringBuilder();
		for(Process p : processes) {
			String str = p.getName();
			String id = "";
			if(str != null) id = str.replaceAll("[^0-9]", "");
			if(id.equals("") || p.getRemaining() == 0) id = "0";
			for(int i = 0; i < p.getSize() / 100; i++) sb.append(id);
		}
		return sb.toString();
	}
	
	public void decrementar(){
		//Le quito una unidad de tiempo restante a todos los procesos que hay en la memoria
		for(Process p : processes) p.decrease();
		
		//Compruebo los huecos que se hayan creado y los acoplo
		for(int i = 0; i < processes.size(); i++) {
			if(processes.get(i).getRemaining() == 0) { //Solo hacemos el acople si el actual es un hueco
				int next = i + 1;
				while(next < processes.size() && processes.get(next).getRemaining() == 0) {
					//Si el siguiente es otro hueco le sumo su tamaño al hueco actual y lo elimino
					processes.get(i).setSize(processes.get(i).getSize() + processes.get(next).getSize());
					processes.remove(next); //Al ir eliminando, el next sigue siendo el que tengo depués de mí
				}
			}
		}
	}
	
	private void agregarProceso(Process nuevo, int pos) {
		//El nuevo proceso empieza donde empezaba el hueco encontrado
		nuevo.setStart(processes.get(pos).getStart());
		//El hueco encontrado empieza despues del proceso.
		processes.get(pos).setStart(processes.get(pos).getStart() + nuevo.getSize());
		//Al hueco encontrado le reduzco el tamaño por el que tiene el proceso.
		processes.get(pos).setSize(processes.get(pos).getSize() - nuevo.getSize());
		//Si ha dado la casualidad de que hemos llenado todo el hueco, eliminamos ese hueco
		if(processes.get(pos).getSize() == 0) processes.remove(pos);
		//Finalmente, añadimos el proceso que hemos creado en el orden que le corresponde
		processes.add(pos, nuevo);
	}
	
	public boolean primerHueco(Process nuevo){
		int pos = -1;

		for(int i = 0; i < processes.size() && pos != -1; i++) //Recorro todos los procesos que tengo hasta encontrar un hueco
			if(processes.get(i).getRemaining() == 0 && nuevo.getSize() <= processes.get(i).getSize()) //Si hay algún hueco donde lo puedo meter
				pos = i; //Me guardo la posicion
		
		if(pos != -1) agregarProceso(nuevo, pos);
		return pos != -1 ? true : false;
	}
	
	public boolean mejorHueco(Process nuevo){
		int posMejor = -1;
		
		for(int i = 0; i < processes.size(); i++) //Recorro todos los procesos que tengo
			if(processes.get(i).getRemaining() == 0 && nuevo.getSize() <= processes.get(i).getSize()) //Si hay un hueco donde lo puedo meter
				if(posMejor == -1) posMejor = i; //Si aun no he encontrado ninguna mejor, la asigno como la primera
				//Si no, miro a ver si es menor que la que tengo
				else if(processes.get(i).getSize() - nuevo.getSize() < processes.get(posMejor).getSize() - nuevo.getSize()) posMejor = i;
		
		if(posMejor != -1) agregarProceso(nuevo, posMejor);
		return posMejor != -1 ? true : false;
	}
}