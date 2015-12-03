package main;

import java.awt.Color;
import java.awt.EventQueue;
import java.util.ArrayList;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.border.EmptyBorder;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFileChooser;

import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.awt.event.ActionEvent;

@SuppressWarnings("serial")
public class Windows extends JFrame {
	private JPanel contentPane;
	
	public static final int FIRSTPLACE = 1;
	public static final int BESTPLACE = 2;
	public static int option; //Algoritmo elegido en el menú
	public static ArrayList<Process> fp = new ArrayList<Process>(); //Procesos extraídos del fichero de lectura. No tienen por qué estar en memoria 
	public static Memory memory = new Memory();
	public static String inputFileName, outputFileName; //Rutas de los ficheros
	
	//Escojo un fichero con JFileChooser (ventana Abrir Archivo) y devuelvo la ruta del archivo escogido
	private String setInputFileName() {
		String path;
		JFileChooser chooser = new JFileChooser(System.getProperty("user.dir"));
		if(chooser.showOpenDialog(contentPane) == JFileChooser.APPROVE_OPTION) {
			File f = chooser.getSelectedFile();
			path = f.getAbsolutePath();
		}
		else path = "";
		return path;
	}
	
	//Escojo un fichero con JFileChooser (ventana Guardar Archivo) y devuelvo la ruta del archivo escogido
	private String setOutputFileName() {
		String path;
		JFileChooser chooser = new JFileChooser(System.getProperty("user.dir"));
		if(chooser.showSaveDialog(contentPane) == JFileChooser.APPROVE_OPTION) {
			File f = chooser.getSelectedFile();
			path = f.getAbsolutePath();
		}
		else path = "";
		return path;
	}
	
	//Pinta una fila de rectángulos con color en función
	//del proceso que esté ocupando ese bloque de memoria (va de 100 en 100)
	private void displayMemory(int moment, String map) {
		Color colors[] = { //Hasta 12 procesos (si se pasa, repite color)
				Color.white, Color.green, Color.blue,
				Color.red, Color.yellow, Color.orange,
				Color.cyan, Color.gray, Color.magenta,
				Color.pink, Color.black, Color.darkGray };
		//Creo un índice del momento en el que estoy
		JLabel rowNum = new JLabel(String.valueOf(moment));
		rowNum.setOpaque(true);
		rowNum.setBounds(12, 120+40*moment, 20, 15); //Le asigno la posición con la altura en función del momento en el que estemos
		contentPane.add(rowNum);
		for(int i = 0; i < 20; i++) {
			JLabel label = new JLabel();
			label.setOpaque(true);
			label.setBounds(35+20*i, 120+40*moment, 15, 15); //Le asigno la posición en función del momento y el bloque en el que estemos
			label.setBorder(BorderFactory.createLineBorder(Color.black,1)); //Le asigno un borde negro
			//Le asigno el fondo poniendo como color la posicion del array colors siendo la posicion
			//el numero que tiene el nombre del proceso
			label.setBackground(colors[Character.getNumericValue(map.charAt(i))%12]);
			contentPane.add(label);
		}
	}
	
	//Leo el fichero de entrada y añado el proceso leído de cada línea al Array de procesos leídos del fichero (fp)
	private void readFile(String fileName) throws IOException {
		try(BufferedReader br = new BufferedReader(new FileReader(fileName))) {
			for(String line; (line = br.readLine()) != null && !line.equals(""); ) fp.add(new Process(line));
		}
	}
	
	//Recorro todos los procesos leídos y les voy asigando un hueco en función del algoritmo elegido
	private void export(int option) throws FileNotFoundException, UnsupportedEncodingException {
		boolean inserted = false;
		PrintWriter out = new PrintWriter(outputFileName, "UTF-8");
		
		//Si se ejecuta cualquier algoritmo por segunda vez, debemos de borrar todo lo que hayamos dibujado 
		contentPane.removeAll();
		initButtons(); //Pero los botones los volvemos a pintar
		
		for(int moment = 1; !fp.isEmpty() || !memory.isEmpty(); moment++) {
			//Recorro la lista de procesos.
			for(int i = 0; i < fp.size(); i++)
				if(fp.get(i).getArrival() <= moment) { //Para aquellos que todavía no han terminado ejecutaré el algoritmo seleccionado
					if(option == FIRSTPLACE) inserted = memory.primerHueco(fp.get(i));
					else if(option == BESTPLACE) inserted = memory.mejorHueco(fp.get(i));
					
					if(inserted) fp.remove(i--); //Si lo consigo meter, lo quito de la lista de entrada y reajusto la posicion del vector
					else System.err.println(fp.get(i).getName() + " no pudo ser cargado en el instante " + moment);						
				}
			displayMemory(moment, memory.toMap()); //Genero los rectángulos que representan la memoria
			contentPane.repaint(); //Dibujo lo generado
			out.println(moment + ": " + memory.toString()); //Me llevo el resultado del instante al fichero de salida
			System.out.println(moment + ": " + memory.toString()); //También lo imprimo por consola
			memory.decrementar(); //Quito un instante de ejecucion a todos los procesos vivos
		}
		out.close(); //OJO
	}
	
	//Inicializo los botones de la ventana principal
	private void initButtons() {
		JButton btnEntrada = new JButton("Entrada");
		btnEntrada.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(!(inputFileName = setInputFileName()).equals(""))
					System.out.println("Entrada: " + inputFileName);
				else System.err.println("No se ha especificado ningún fichero de entrada");
			}
		});
		btnEntrada.setBounds(12, 32, 117, 25);
		contentPane.add(btnEntrada);
		
		JButton btnSalida = new JButton("Salida");
		btnSalida.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(!(outputFileName = setOutputFileName()).equals(""))
					System.out.println("Salida: " + outputFileName);
				else System.err.println("No se ha especificado ningún fichero de salida");
			}
		});
		btnSalida.setBounds(12, 69, 117, 25);
		contentPane.add(btnSalida);
		
		JButton btnPrimerHueco = new JButton("Primer Hueco");
		btnPrimerHueco.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					readFile(inputFileName);
					export(FIRSTPLACE);
				} catch (Exception e1) {
					JOptionPane.showMessageDialog(contentPane, "Por favor, especifica los archivos de entrada/salida",
							"Error en los ficheros", JOptionPane.ERROR_MESSAGE);
					fp.clear(); //Borramos lo que hemos leído (si es que lo hemos hecho)
				}
			}
		});
		btnPrimerHueco.setBounds(141, 32, 139, 62);
		contentPane.add(btnPrimerHueco);
		
		JButton btnMejorHueco = new JButton("Mejor Hueco");
		btnMejorHueco.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					readFile(inputFileName);
					export(BESTPLACE);
				} catch (IOException e1) {
					JOptionPane.showMessageDialog(contentPane, "Por favor, especifica los archivos de entrada/salida",
							"Error en los ficheros", JOptionPane.ERROR_MESSAGE);
					fp.clear(); //Borramos lo que hemos leído (si es que lo hemos hecho)
				}
			}
		});
		btnMejorHueco.setBounds(292, 32, 139, 62);
		contentPane.add(btnMejorHueco);
	}
	
	//Inicializo el Frame
	public Windows() {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (ClassNotFoundException | InstantiationException | IllegalAccessException
				| UnsupportedLookAndFeelException e2) {
			System.err.println("No se ha podido establecer el tema predeterminado de tu SO");
		}
		
		setTitle("Gestor de memoria SO");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 450, 600);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(null);
		initButtons();
	}

	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					Windows frame = new Windows();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
}
