class Bunker {
    private Integer posX;
    private Integer posY;
    private Integer health;

    public Bunker(Integer posX, Integer posY) {
        this.posX = posX;
        this.posY = posY;
        this.health = 100;
    }

    public Integer getPosX() {
        return posX;
    }

    public Integer getPosY() {
        return posY;
    }

    public Integer getHealth() {
        return health;
    }


}