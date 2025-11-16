# GE - Gestion des Employés

Application Qt C++ pour la gestion des employés avec statistiques et authentification sécurisée.

## Fonctionnalités

- **Authentification** : Connexion via CIN et mot de passe.
- **Réinitialisation de Mot de Passe** : Via code de vérification envoyé par email (Brevo API).
- **Gestion des Employés** : Interface pour gérer les employés (via `mainwindow`).
- **Statistiques** : Graphiques et rapports (Qt Charts).
- **Base de Données** : Utilise SQLite ou autre via Qt SQL.

## Prérequis

- **Qt 6.7.3** ou supérieur (avec MinGW 64-bit).
- **OpenSSL** : Pour les connexions HTTPS (DLLs `libssl-3-x64.dll` et `libcrypto-3-x64.dll` à côté de l'exécutable).
- **Brevo Account** : Pour l'envoi d'emails (optionnel en mode mock).

## Installation et Compilation

1. **Cloner ou ouvrir le projet** dans Qt Creator.
2. **Configurer le kit** : Qt 6.7.3 MinGW 64-bit.
3. **Construire** :
   - Ouvrir `GE.pro` dans Qt Creator.
   - Build > Build Project.
   - Ou via ligne de commande :
     ```powershell
     qmake GE.pro
     mingw32-make
     ```
4. **Exécuter** : Lancer `GE.exe` depuis le dossier `build\Desktop_Qt_6_7_3_MinGW_64_bit-Debug\debug\`.

### Script Automatique (Windows)

Utilisez `build_and_run.ps1` pour nettoyer, compiler et exécuter :
```powershell
.\build_and_run.ps1 -UseMock  # Mode test sans API Brevo
```

## Configuration

### Base de Données

Le projet utilise Qt SQL. Configurez la connexion dans `connectdb.cpp`.

Tables requises :
- `Users` : `cin VARCHAR(8) PRIMARY KEY, mdp VARCHAR(255), email VARCHAR(255)`
- `EMAIL` : `cin VARCHAR(8) PRIMARY KEY, email VARCHAR(255) NOT NULL`
- `VerificationCodes` : Créée automatiquement (`cin, code, expires_at`)

### Envoi d'Emails (Brevo)

Pour activer l'envoi de codes de vérification :

1. Créez un compte sur [Brevo](https://www.brevo.com/).
2. Récupérez une clé API v3 avec droits "SMTP transactional".
3. Vérifiez une adresse expéditrice.
4. Définissez les variables d'environnement :
   ```powershell
   $env:BREVO_API_KEY = 'votre_cle_api'
   $env:BREVO_SENDER_EMAIL = 'sender@domaine.com'
   ```

### Mode Test (Mock)

Pour tester sans API Brevo :
```powershell
$env:BREVO_USE_MOCK = '1'
```
Le code s'affiche dans une boîte de dialogue au lieu d'être envoyé.

## Utilisation

### Connexion
- Entrez CIN (8 chiffres) et mot de passe.
- Cliquez "login".

### Réinitialisation de Mot de Passe
1. Cliquez "mdp oubliee".
2. Entrez votre CIN.
3. Un code de vérification est envoyé par email (ou affiché en mode mock).
4. Entrez le code dans la boîte de dialogue pour vous connecter ou réinitialiser le mot de passe.

### Bouton "J'ai un Code"
- Permet d'entrer manuellement un CIN et un code reçu pour se connecter.

## Sécurité

- **Codes de Vérification** : Expirent après 10 minutes, stockés temporairement.
- **Secrets** : Ne commitez jamais les clés API ou mots de passe. Utilisez des variables d'environnement.
- **Mots de Passe** : Stockés en clair (considérez le hachage pour la production).
- **Emails** : Utilisez des adresses vérifiées et des API sécurisées.

## Dépannage

### Erreurs de Compilation
- Vérifiez que Qt et MinGW sont installés.
- Nettoyez le dossier `build` : `Remove-Item -Recurse -Force .\build`.

### Erreurs d'Envoi d'Email
- Vérifiez les variables d'environnement.
- Assurez-vous que l'adresse expéditrice est vérifiée sur Brevo.
- En mode mock, le code s'affiche directement.

### Erreurs de Base de Données
- Vérifiez la connexion dans `connectdb.cpp`.
- Assurez-vous que les tables existent.

### DLL Manquantes
- Copiez `libssl-3-x64.dll` et `libcrypto-3-x64.dll` à côté de `GE.exe`.

## Structure du Projet

- `connection.*` : Interface de connexion et réinitialisation.
- `mainwindow.*` : Gestion des employés.
- `brevo_sender.*` : Envoi d'emails via Brevo.
- `verifycodedialog.*` : Boîte de dialogue de vérification.
- `employee.*` : Logique métier des employés.
- `build_and_run.ps1` : Script de build automatique.

## Contribution

- Utilisez Qt Creator pour l'édition.
- Testez en mode mock avant la production.
- Suivez les bonnes pratiques Qt (signaux/slots, gestion mémoire).

## Licence

[Ajoutez votre licence ici, par exemple MIT.]
````

