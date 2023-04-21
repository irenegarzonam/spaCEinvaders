import java.net.ServerSocket;
import java.sql.SQLOutput;
import java.util.*;

/**
 * Clase juego, es la que maneja toda la logica y funcionamiento del juego
 */
public class Game implements Observer {
    private Integer score;
    private Integer lives;
    private final Alien[][] aliens;
    private final List<Bunker> bunkers;
    private Ovni ovni;
    private final ServerSocket server;
    boolean activeGame;
    boolean reachedRightCorner;
    private String xPlayer;
    private String xBullet;
    private String yBullet;

    /**
     * Metodo constructor de la clase juego, inicializa todas los atributos del juego
     * @param server recibe el servidor por el cuel envia y recibe informacion
     */
    public Game(ServerSocket server) {
        score = 0;
        lives = 2;
        aliens = new Alien[5][12];
        bunkers = new ArrayList<Bunker>();
        this.server = server;
        this.activeGame = true;
        this.reachedRightCorner = false;
        this.ovni = new Ovni();
        createAliens(5, 12, 55, 75, 15);
        createBunkers();
    }

    /**
     * Actualiza todas las posiciones de los aliens y envia constantemente la informacion necesaria al cliente
     * @return el String que se debe enviar al cliente
     */
    public String updateGame() {
            boolean flag = this.reachedRightCorner;
            // Actualizar la posición de los extraterrestres, los ovnis y los disparos
            // Verifica si algun alien llego al final de la pantalla
            for (Integer i = 0; i < aliens.length; i++) {
                for (Integer j = 0; j < aliens[0].length; j++) {
                    if(flag == this.reachedRightCorner) {
                        if (aliens[i][j] != null) {
                            aliens[i][j].move();
                            if (aliens[i][j].getPosY() >= 700) {//1000 es el screen Height
                                lives--;
                                aliens[i][j].deleteObserver(this);
                                aliens[i][j] = null;
                            }
                        }
                    }
                }
                if (getRandomNumber() % 7 == 0) {
                    createOvni();
                }
                if (this.ovni.exists) {
                    this.ovni.move();
                }
            }
            verifyWin();
        return generateFinalString();
        }

    /**
     * cambia el valor del booleano por su contrario
     */
    void setReachedRightCorner(){
        this.reachedRightCorner = !this.reachedRightCorner;
    }

    /**
     * crea la matriz de aliens
     * @param numRows numero de filas de la matriz
     * @param numCols numero de columnas de la matriz
     * @param x posicion en x donde va a empezar la matrix
     * @param y posicion en y donde empieza la matriz
     * @param speed velocidad de los aliens
     */
    public void createAliens(Integer numRows, Integer numCols, Integer x, Integer y, Integer speed){
        for (Integer i = 0; i < numRows; i++) {
            for (Integer j = 0; j < numCols; j++) {
                this.aliens[i][j] = new Alien(this, x + j * 80, y + i * 80, speed);
                this.aliens[i][j].addObserver(this);
            }
        }
    }

    /**
     * Encuentra en que posicion de la matriz esta un alien a partir de sus coordenadas
     * @param xCoords coordenadas en x del alien
     * @param yCoords coordenadas en y del alien
     * @return lista con las posiciones x y y del alien
     */
    public List<Integer> findPosInMatrixAlien(Integer xCoords, Integer yCoords) {
        List<Integer> intList = new ArrayList<>();
        for (Integer i = 0; i < aliens.length; i++) {
            for (Integer j = 0; j < aliens[0].length; j++) {
                if(this.aliens[i][j].getPosX() == xCoords && this.aliens[i][j].getPosY() == yCoords){
                    intList.add(i);
                    intList.add(j);
                    return intList;
                }
            }
        }
        return intList;
    }

    /**
     * Saca de la pantalla el alien eliminado
     * @param Row Fila de la matriz en la que se encuentra el alien
     * @param Column columna de la matriz en la que esta el alien
     */
    public void deleteAlien(Integer Row, Integer Column) {
        aliens[Row][Column].setPosY(aliens[Row][Column].getPosY() + 1000);
    }

    /**
     * Retorna un alien "eliminado" a su posicion original
     * @param Row fila de la matriz en la que estaba el alien
     * @param Column columna de la matriz donde esta el alien
     */
    public void bringBackAlien(Integer Row, Integer Column) {
        if(aliens[Row][Column].getPosY() > 1000){
            aliens[Row][Column].setPosY(aliens[Row][Column].getPosY() - 1000);
        }
    }

    /**
     * Crea la lista de bunkers
     */
    public void createBunkers() {
        for (Integer i = 1; i <= 4; i++) {
            Bunker newBunker = new Bunker((1100 / 4) * i - 75, 500);
            bunkers.add(newBunker);
        }
    }

    /**
     * En caso de no haber un ovni, crea uno
     */
    public void createOvni(){
        if(!this.ovni.exists){
            this.ovni = new Ovni(8);
        }
    }

    /**
     * Se encarga de mantener actualizado a los observers de todos los aliens
     * @param o     the observable object.
     * @param arg   an argument passed to the {@code notifyObservers}
     *                 method.
     */
    @Override
    public void update(Observable o, Object arg) {
        if (arg instanceof String && arg.equals("move")) {
            updateSpeed();
        }
    }

    /**
     * actualiza la velocidad y posicion de todos los aliens en caso de que alguno cambie
     */
    private void updateSpeed() {
        for (int row = 0; row < aliens.length; row++) {
            for (int col = 0; col < aliens[0].length; col++) {
                //if(aliens[row][col] != null) {
                    aliens[row][col].change();
                //}
            }
        }
    }

    /**
     * Genera un numero random entre 1 y 26
     * @return numero random generado
     */
    public static Integer getRandomNumber() {
        Random rand = new Random();
        Integer randomInt = rand.nextInt(26) + 1; // Generates a random number between 1 and 25
        return randomInt;
    }

    /**
     * Verifica si la matriz de aliens esta vacia
     */
    public void verifyWin() {
        boolean win = true;
        for (Integer i = 0; i < aliens.length; i++) {
            for (Integer j = 0; j < aliens[0].length; j++) {
                if (aliens[i][j].getPosY() < 1000) {
                    win = false;
                    break;
                }
            }
        }
        if (win) {
            this.lives += 1;
            this.score += 100;
        }
    }

    /**
     * Convierte la matriz de aliens en un String para enviarle al cliente
     * @return String de la matriz
     */
    String generateAliensMatrix() {
        Integer[][][] matrix = new Integer[aliens.length][aliens[0].length][2];
        for (int row = 0; row < aliens.length; row++) {
            for (int col = 0; col < aliens[0].length; col++) {
                if(aliens[row][col] != null) {
                    matrix[row][col][0] = this.aliens[row][col].getPosX();
                    matrix[row][col][1] = this.aliens[row][col].getPosY();
                }else{
                    break;
                }
            }
        }
        String matrixAsString = Arrays.deepToString(matrix);
        return matrixAsString;
    }

    /**
     * Convierte la lista de bunkers en un string para enviar al cliente
     * @return String a enviar
     */
    String generateBunkersMatrix() {
        String Bunkers = "[";
        for (Integer i = 0; i < bunkers.size(); i++) {
            Integer x = bunkers.get(i).getPosX();
            Integer y = bunkers.get(i).getPosY();
            Integer health = bunkers.get(i).getHealth();
            String add = "[" + x + "," + y + "," + health + "],";
            Bunkers += add;
        }
        Bunkers += "]";
        //System.out.println(Bunkers);
        return Bunkers;
    }

    /**
     * Convierte las posiciones del ovni en una lista string para enviar al cliente
     * @return String a enviar
     */
    String generateOvniString() {
        String Ovni;
        if(this.ovni.exists) {
            Ovni = "[" + this.ovni.getPosx() + "," + this.ovni.getPosy() + "]";
        }else{
            Ovni = "[9000,9000]";
        }
        return Ovni;
    }

    /**
     * Concatena todos los String necesarios para pasarle al cliente
     * @return String con todos los datos
     */
    String generateFinalString() {
        String Score = Integer.toString(this.score);
        String Lives = Integer.toString(this.lives);
        String FinalString = Score + "_" + Lives + "_" + generateBunkersMatrix() + "_" + generateAliensMatrix()+"_"+generateOvniString()+
                "_" + this.xPlayer + "_" + this.xBullet + "_" + this.yBullet;
        return FinalString;
    }

    /**
     * Maneja los datos del alien con el que la bala tuvo colision
     * @param coordX coordenada en x del alien
     * @param coordY coordenada en y del alien
     * @param type tipo de alien
     */
    void manageAlienHit(Integer coordX, Integer coordY, Integer type){
        List<Integer> matrixPos= findPosInMatrixAlien(coordX, coordY);
        if(type == 1){
            this.score += 10;
        }else if(type == 2){
            this.score += 20;
        }else if(type == 3){
            this.score += 40;
        }
        if(coordX != 0 || coordY != 0) {
            deleteAlien(matrixPos.get(0), matrixPos.get(1));
        }
    }

    /**
     * Administra todo la informacion recibida por el cliente
     * @param data String con datos recibidos
     */
    void manageReceivedData(String[] data){
        Integer xCoord = Integer.parseInt(data[1]);
        Integer yCoord = Integer.parseInt(data[2]);
        Integer alienType = Integer.parseInt(data[3]);
        manageAlienHit(xCoord, yCoord, alienType);

        Integer hitBunker = Integer.parseInt(data[4]); //-1;
        this.bunkers.get(hitBunker).gotHit();
        this.xPlayer = data[5];
        this.xBullet = data[6];
        this.yBullet = data[7];
    }
}
