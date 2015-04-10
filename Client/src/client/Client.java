/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package client; // package

import java.awt.event.ActionEvent; // librerie
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import javax.swing.JOptionPane;
import java.util.Date;

/**
 *
 * @author Andrea
 */
public final class Client extends javax.swing.JFrame { // classe con l'estensione swing per l'interfaccia

    /**
     * Creates new form NewJFrame
     */
    private String messSend;

    public Client() throws IOException {
        initComponents(); //funzione che inizializza i componenti
        this.setLocation(500, 200); // posiziono nello schermo
        this.setSize(520, 677); // do le dimensioni alla jframe       
        this.setTitle("CLIENT CHAT"); // setto il titolo
        textField.setEditable(false); // disabilito input
        messageArea.setEditable(false); // disabilito input
        messageArea2.setEditable(false); //disabilito input nella seconda message area
        openChat(); // chiamo la funzione per l'inserimento delle vecchie chat 
        textField.addActionListener(new ActionListener() { // azione dell'utente

            public void actionPerformed(ActionEvent e) { // quando premo invio fa questo
                messSend = textField.getText(); // associo a messSend il contenuto scritto nella textField
                Date date = new Date(); // creo un oggetto date per inserire la data               
                output.println(date.toString().substring(11, 16) + "  " + messSend); // INVIO IL MESSAGGIO con aggiunta l'ora
                textField.setText(""); // RESETTO IL TESTO
            }
        });
    }
    BufferedReader input; // dichiaro un buffer reader per gli input
    PrintWriter output; // dichiaro un print writer per gli output

    private String getServerAddress() {
        return JOptionPane.showInputDialog( // la funzione torna il valore preso in input
                this, // primo paramento il Jframe
                "Inserisci l'ip del server:", // output
                "Benvenuto nella chat", //titolo
                JOptionPane.QUESTION_MESSAGE); // tipo di dialog creata
    }

    public String getName() {
        return JOptionPane.showInputDialog( // la funzione torna il valore preso in input
                this, // primo paramento il Jframe
                "Inserisci il tuo nome:", //output
                "Benvenuto nella chat", // titolo
                JOptionPane.PLAIN_MESSAGE); // tipo di dialog creata
    }

    public int getPort() {
        return Integer.parseInt(JOptionPane.showInputDialog( // la funzione torna il valore preso in input
                this, // primo paramento il Jframe
                "Inserisci la porta:", //output
                "Benvenuto nella chat", // titolo
                JOptionPane.PLAIN_MESSAGE)); // tipo di dialog creata
    }

    void run() throws IOException { // metodo run chiamato dal main

        String serverAddress = getServerAddress(); //prendo in input l'ip del server
        int port = getPort(); // prendo in input la porta
        Socket socket = new Socket(serverAddress, port);//passo come parametri al socket ip e porta presi in input precedentemente
        if (serverAddress.equals("127.0.0.1"))//se l'ip è 127.0.0.
        {
            label.setText("Stai utilizando la porta: " + port + " e sei connesso in locale"); // scrive il messaggio nella label
        } else {
            label.setText("Stai utilizando la porta: " + port + " e sei connesso a " + serverAddress); // se non è locale scrive l'ip
        }
        input = new BufferedReader(new InputStreamReader( //dichiaro un buffer reader per prendre il input messaggi dal server
                socket.getInputStream()));
        output = new PrintWriter(socket.getOutputStream(), true); //dichiaro un print writer per inviare messaggi al server

        while (true) { // while in loop
            String linea = input.readLine();  // leggo la nuova linea ricevuta
            if (linea.equals("START_NAME")) { // se c'è scritto START_NAME
                String name = getName(); // setta il nome del client
                Date date = new Date(); // creo un oggetto date per inserire la data
                this.setTitle(name.toUpperCase() + " CHAT " + date.toString().substring(0, 9) + "  " + date.toString().substring(24, 28)); // cambia il titolo in base al nome e alla data
                output.println(name);  // invio al server il nome dell'utente
            } else if (linea.equals("NameAccepted")) { // se il nome è stato accettato 
                textField.setEditable(true); //riabilita la textfield
            } else if (linea.contains("MessageS")) { //se la stringa contiene MessageS               
                messageArea.append(linea.substring(linea.indexOf("Message") + 9) + "\n"); // scrive il messaggio 
                insertChat(true, linea.substring(linea.indexOf("Message") + 9)); // funzione per scrivere su file
                messageArea2.append("\n"); // spazio nell'altra text area
            } else if (linea.contains("Message")) { //se la stringa contiene Message
                messageArea2.append(linea.substring(linea.indexOf("Message") + 8) + "\n"); // scrive il messaggio
                insertChat(false, linea.substring(linea.indexOf("Message") + 8)); // funzione per scrivere su file
                messageArea.append("\n"); // spazio nell'altra text area
            }
        }
    }

    public void openChat() throws IOException {
        BufferedReader reader; // dichiaro un buffer
        reader = new BufferedReader(new FileReader("chat.txt")); // creo o sovrascrivo chat.txt
        int k = 0; // pongo k a 0
        String line = reader.readLine(); // line diventa uguale alla prima linea
        while (line != null) { // finchè ci sono linee
            messageArea.append(line); // scrivo nella text area la linea
            messageArea.append("\n"); // vado a capo
            k++; // incremento k di 1
            line = reader.readLine(); // pongo line alla linea successiva
        } // fine del while
        reader = new BufferedReader(new FileReader("chat2.txt"));// creo o sovrascrivo chat.txt
        k = 0;// pongo k a 0
        line = reader.readLine(); // line diventa uguale alla prima linea
        while (line != null) { // finchè ci sono linee
            messageArea2.append(line);// scrivo nella text area la linea
            messageArea2.append("\n");// vado a capo
            k++;// incremento k di 1
            line = reader.readLine();// pongo line alla linea successiva
        }// fine del while
    }

    public void insertChat(boolean flag, String message) throws IOException {  // funzione per l'inserimento in file dei dati          
        BufferedWriter bfw,bfw2; //dichiaro buffer
        if (flag) {
            bfw = new BufferedWriter(new FileWriter("chat.txt", true));  //creo buffer
            bfw2 = new BufferedWriter(new FileWriter("chat2.txt", true));  //creo buffer
        } else {
            bfw = new BufferedWriter(new FileWriter("chat2.txt", true)); //creo buffer
            bfw2 = new BufferedWriter(new FileWriter("chat.txt", true)); //creo buffer
        }
        bfw.write(message); // inserisco messaggio nel file
        bfw.newLine(); // vado a capo nel file
        bfw2.newLine(); // vado a capo nel file 2
        bfw.close();  // chiudo il buffer      
        bfw2.close(); // chiudo il buffer 
    }

    public static void main(String[] args) throws Exception { // main (dove inizia il programma)
        Client client = new Client(); // creo un nuovo client
        client.setVisible(true); // rende visibile la GUI
        client.run(); //chiamo il metodo run
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked") // quello che fa il compilatore
    // <editor-fold defaultstate="collapsed" desc="Generated Code">                          
    private void initComponents() {

        textField = new javax.swing.JTextField();
        jScrollPane1 = new javax.swing.JScrollPane();
        messageArea = new javax.swing.JTextArea();
        label = new javax.swing.JLabel();
        jScrollPane2 = new javax.swing.JScrollPane();
        messageArea2 = new javax.swing.JTextArea();

        setDefaultCloseOperation(javax.swing.WindowConstants.DO_NOTHING_ON_CLOSE);
        setResizable(false);
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                formWindowClosing(evt);
            }
        });
        getContentPane().setLayout(null);

        textField.setPreferredSize(new java.awt.Dimension(500, 50));
        getContentPane().add(textField);
        textField.setBounds(0, 591, 500, 39);

        messageArea.setBackground(new java.awt.Color(204, 255, 255));
        messageArea.setColumns(20);
        messageArea.setRows(5);
        messageArea.setBorder(null);
        jScrollPane1.setViewportView(messageArea);

        getContentPane().add(jScrollPane1);
        jScrollPane1.setBounds(0, 29, 250, 556);
        getContentPane().add(label);
        label.setBounds(0, 0, 500, 19);

        messageArea2.setBackground(new java.awt.Color(204, 255, 255));
        messageArea2.setColumns(20);
        messageArea2.setRows(5);
        messageArea2.setBorder(null);
        jScrollPane2.setViewportView(messageArea2);

        getContentPane().add(jScrollPane2);
        jScrollPane2.setBounds(250, 29, 250, 556);

        pack();
    }// </editor-fold>                        

    private void formWindowClosing(java.awt.event.WindowEvent evt) {                                   
        System.exit(0); // chiude il programma
    }                                  

    /**
     * @param args the command line arguments
     */

    // Variables declaration - do not modify                     
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JLabel label;
    private javax.swing.JTextArea messageArea;
    private javax.swing.JTextArea messageArea2;
    private javax.swing.JTextField textField;
    // End of variables declaration                   

}
