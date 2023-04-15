import java.io.*;
import java.net.*;

public class SpaceInvadersClientHandler extends Thread {
    private Socket clientSocket; // El socket de comunicación con el cliente
    private PrintWriter out; // Salida para enviar mensajes al cliente
    private BufferedReader in; // Entrada para recibir mensajes del cliente
    private SpaceInvadersServer server; // Referencia al servidor para manejar la lógica del juego

    /**
     * Constructor de la clase SpaceInvadersClientHandler. Recibe como parámetros el socket
     * de comunicación con el cliente y la referencia al servidor.
     * @param socket El socket de comunicación con el cliente.
     * @param server La referencia al servidor.
     */
    public SpaceInvadersClientHandler(Socket socket, SpaceInvadersServer server) {
        this.clientSocket = socket;
        this.server = server;

        // Obtiene las entradas y salidas del socket para comunicarse con el cliente
        try {
            out = new PrintWriter(clientSocket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Método run de la clase Thread que se ejecuta cuando se inicia la instancia del thread.
     * Recibe los mensajes del cliente y actualiza el estado del juego correspondiente.
     */
    public void run() {
        String inputLine;
        try {
            while ((inputLine = in.readLine()) != null) {
                // Procesa los mensajes recibidos del cliente
                server.processMessage(inputLine, this);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Método para enviar un mensaje al cliente.
     * @param message El mensaje a enviar.
     */
    public void sendMessage(String message) {
        out.println(message);
    }
}

