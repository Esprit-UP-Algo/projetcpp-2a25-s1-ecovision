
-- Table des employés
CREATE TABLE EMPLOYE (
    id NUMBER PRIMARY KEY,
    cin VARCHAR2(20) UNIQUE,
    nom VARCHAR2(100) NOT NULL,
    prenom VARCHAR2(100),
    adresse VARCHAR2(200),
    telephone VARCHAR2(20),
    poste VARCHAR2(100),
    presence VARCHAR2(20),
    salaire NUMBER(10,2)
);

-- Table des locaux
CREATE TABLE LOCAL (
    id_local NUMBER PRIMARY KEY,
    type_local VARCHAR2(100),
    ville VARCHAR2(100),
    adresse VARCHAR2(200),
    disponibilite VARCHAR2(50),
    nom_proprietaire VARCHAR2(100),
    numero VARCHAR2(50),
    prix NUMBER(10,2),
    cin VARCHAR2(20),
    CONSTRAINT fk_local_proprietaire FOREIGN KEY (cin) REFERENCES EMPLOYE(cin)
);

-- Table des résidents
CREATE TABLE RESIDENT (
    id_resident NUMBER PRIMARY KEY,
    nom VARCHAR2(100),
    prenom VARCHAR2(100),
    sexe VARCHAR2(10),
    date_naissance DATE,
    etat_civil VARCHAR2(50),
    profession VARCHAR2(100),
    statut_occupation VARCHAR2(100),
    cin VARCHAR2(20),
    CONSTRAINT fk_resident_cin FOREIGN KEY (cin) REFERENCES EMPLOYE(cin)
);

-- ============================================================================
-- TABLE RFID - Gestion des cartes RFID et leurs associations employés
-- ============================================================================
CREATE TABLE RFID_CARDS (
    id NUMBER PRIMARY KEY,
    uid VARCHAR2(50) NOT NULL UNIQUE,
    cin VARCHAR2(20) NOT NULL,
    date_association TIMESTAMP DEFAULT SYSTIMESTAMP,
    statut VARCHAR2(20) DEFAULT 'ACTIVE',
    CONSTRAINT fk_rfid_employe FOREIGN KEY (cin) REFERENCES EMPLOYE(cin)
);

CREATE INDEX idx_rfid_uid ON RFID_CARDS(uid);

-- ============================================================================
-- TABLE POINTAGE - Enregistrement des présences (arrivée/départ)
-- ============================================================================
CREATE TABLE POINTAGE (
    id NUMBER PRIMARY KEY,
    cin VARCHAR2(20) NOT NULL,
    date_arrivee TIMESTAMP,
    date_depart TIMESTAMP,
    CONSTRAINT fk_pointage_employe FOREIGN KEY (cin) REFERENCES EMPLOYE(cin)
);

CREATE INDEX idx_pointage_cin ON POINTAGE(cin);
CREATE INDEX idx_pointage_date ON POINTAGE(date_arrivee);

-- Table des réclamations
CREATE TABLE RECLAMATION (
    code VARCHAR2(50) PRIMARY KEY,
    date_reclamation DATE,
    nom_resident VARCHAR2(200),
    description CLOB,
    etat VARCHAR2(50),
    delai NUMBER
);

-- Table des ressources
CREATE TABLE RESSOURCE (
    code_ressources VARCHAR2(50) PRIMARY KEY,
    type_ressource VARCHAR2(100),
    localisation VARCHAR2(200),
    consommateur VARCHAR2(200),
    date_mesure DATE,
    idresident NUMBER,
    CONSTRAINT fk_ressources_resident FOREIGN KEY (idresident) REFERENCES RESIDENT(id_resident)
);

-- Exemples d'insertion (à adapter)
-- INSERT INTO EMPLOYE (cin, nom, prenom, adresse, telephone, poste, presence, salaire)
-- VALUES ('A12345', 'Doe', 'John', 'Rue Exemple', '0612345678', 'Technicien', 'present', 25000.00);

-- Note : Si votre SGBD n'est pas Oracle, adaptez les types et la syntaxe d'auto-incrément
-- (ex : MySQL -> INT AUTO_INCREMENT, PostgreSQL -> SERIAL ou IDENTITY, SQLite -> INTEGER PRIMARY KEY AUTOINCREMENT).

-- ============================================================================
-- Table UTILISATEUR (authentification avec RFID optionnel)
-- ============================================================================
CREATE TABLE UTILISATEUR (
    cin VARCHAR2(20) PRIMARY KEY,
    mdp VARCHAR2(200) NOT NULL,
    email VARCHAR2(100) UNIQUE,
    verification_code VARCHAR2(50),
    verification_expires_at TIMESTAMP(6),
    uid_rfid VARCHAR2(50) UNIQUE,
    created_at TIMESTAMP DEFAULT SYSTIMESTAMP,
    CONSTRAINT fk_utilisateur_employe FOREIGN KEY (cin) REFERENCES EMPLOYE(cin)
);

-- Remarques :
-- 1) `mdp` doit contenir un hachage sécurisé (bcrypt/argon2). Ne stockez jamais de mots de passe en clair.
-- 2) `uid_rfid` stocke l'UID de la carte RFID associée à cet utilisateur
-- 3) Si vous préférez une clé numérique, remplacez `cin` par un `id_utilisateur` NUMERIC/IDENTITY.
-- 4) Adaptez les types si vous utilisez un autre SGBD (MySQL/Postgres/SQLite).

-- -----------------------------------------------------------------------------
-- Table minimaliste pour recevoir le code de vérification par email
-- Champs : email, verification_code, verification_expires_at
-- -----------------------------------------------------------------------------
CREATE TABLE EMAIL_VERIFICATION (
    email VARCHAR2(255) PRIMARY KEY,
    verification_code VARCHAR2(100),
    verification_expires_at TIMESTAMP
);

CREATE INDEX idx_email_verif_code ON EMAIL_VERIFICATION(verification_code);

-- Exemple : insérer l'email personnel pour recevoir un code (exécuter dans SQL Developer)
-- Remplacez 'VERIFCODE123' par le code réel que l'application stockera.
INSERT INTO EMAIL_VERIFICATION (email, verification_code, verification_expires_at)
VALUES ('amineshimi90@gmail.com', 'VERIFCODE123', SYSTIMESTAMP + INTERVAL '1' HOUR);

-- -----------------------------------------------------------------------------
-- Table Users (compatibilité avec le code de login existant)
-- Utilisée par connection.cpp pour l'authentification
-- -----------------------------------------------------------------------------
CREATE TABLE Users (
    cin VARCHAR2(20) PRIMARY KEY,
    mdp VARCHAR2(255) NOT NULL,
    email VARCHAR2(255),
    verification_code VARCHAR2(10),
    verification_expires_at TIMESTAMP,
    uid_rfid VARCHAR2(50) UNIQUE
);

-- Exemple : créer un compte test pour vous connecter
INSERT INTO Users (cin, mdp, email, uid_rfid)
VALUES ('12345678', 'test123', 'amineshimi90@gmail.com', 'TEST123456ABCD');

-- Note : Pour production, stockez le mot de passe haché, pas en clair.
-- uid_rfid peut être NULL si l'utilisateur n'a pas de carte RFID associée
