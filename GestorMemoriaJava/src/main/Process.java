package main;

import java.util.StringTokenizer;

public class Process {
	private String name; //Nombre del proceso
	private int arrival; //Instante de llegada
	private int size; //Tamaño que ocupa en memoria el proceso
	private int remaining; //Instantes que dura su ejecución
	private int start; //Direccion de memoria inicial	
	
	public String getName() { return name; }
	public int getArrival() { return arrival; }
	public int getSize() { return size; }
	public int getRemaining() { return remaining; }
	public int getStart() { return start; }
	
	public void setName(String name) { this.name = name; }
	public void setArrival(int arrival) { this.arrival = arrival; }
	public void setSize(int size) { this.size = size; }
	public void setRemaining(int remaining) { this.remaining = remaining;}
	public void setStart(int start) { this.start = start; }
	
	public Process() { //Con esto creamos un proceso ficticio que será siempre el último hueco de la memoria
		arrival = remaining = start = 0;
		size = Memory.MAXMEMORIA;
	}
	
	//Tokenizamos el string y asignamos cada campo según el orden del formato
	public Process(String line) {
		StringTokenizer tokens = new StringTokenizer(line);
		setName(tokens.nextToken());
		setArrival(Integer.parseInt(tokens.nextToken()));
		setSize(Integer.parseInt(tokens.nextToken()));
		setRemaining(Integer.parseInt(tokens.nextToken()));
		setStart(0); // la direccion de comienzo en la memoria aun no la sé
	}
	
	public Process(Process f) { //No nos sirve
		name = f.name;
		arrival = f.arrival;
		size = f.size;
		remaining = f.remaining;
		start = 0; // la direccion de comienzo en la memoria aun no la sé
	}
	
	public String toString() {
		StringBuilder sb = new StringBuilder("[" + start);
		if(remaining > 0) sb.append(" " + name + " ");
		else sb.append(" HUECO ");
		sb.append(size + "]");
		return sb.toString();
	}
	
	//Por cada instante que pase en la simulación, deberemos decrementar el tiempo que le queda a cada proceso
	public void decrease() { if(remaining > 0) remaining--; }
}