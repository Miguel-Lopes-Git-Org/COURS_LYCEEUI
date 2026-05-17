CREATE EXTENSION IF NOT EXISTS pgcrypto;

DROP VIEW IF EXISTS getLogin CASCADE;
DROP VIEW IF EXISTS getComptesUtilisateurs CASCADE;
DROP VIEW IF EXISTS getCours CASCADE;
DROP VIEW IF EXISTS getEtudiantsDetails CASCADE;
DROP VIEW IF EXISTS getEtudiants CASCADE;
DROP VIEW IF EXISTS getEnseignantsDetails CASCADE;
DROP VIEW IF EXISTS getEnseignants CASCADE;
DROP VIEW IF EXISTS getFormations CASCADE;
DROP VIEW IF EXISTS getMatieres CASCADE;
DROP VIEW IF EXISTS getVilles CASCADE;

DROP TABLE IF EXISTS etudiant_cours CASCADE;
DROP TABLE IF EXISTS enseignant CASCADE;
DROP TABLE IF EXISTS etudiant CASCADE;
DROP TABLE IF EXISTS formation CASCADE;
DROP TABLE IF EXISTS cours CASCADE;
DROP TABLE IF EXISTS matiere CASCADE;
DROP TABLE IF EXISTS personne CASCADE;
DROP TABLE IF EXISTS ville CASCADE;

CREATE TABLE ville(
    id SERIAL PRIMARY KEY,
    nom VARCHAR(255) NOT NULL,
    codePostal VARCHAR(5) NOT NULL
);

CREATE TABLE personne (
    id SERIAL PRIMARY KEY,
    nom VARCHAR(255) NOT NULL,
    prenom VARCHAR(255) NOT NULL,
    username VARCHAR(255) UNIQUE,
    password VARCHAR(255) NOT NULL,
    password_salt VARCHAR(255),
    needPasswordChange BOOLEAN DEFAULT TRUE,
    ville_id INTEGER,
    sexe VARCHAR(1),
    isAdmin BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (ville_id) REFERENCES ville(id)
);

CREATE TABLE matiere (
    id SERIAL PRIMARY KEY,
    nom VARCHAR(255) NOT NULL,
    alias VARCHAR(255) NOT NULL
);

CREATE TABLE cours (
    id SERIAL PRIMARY KEY,
    nom VARCHAR(255) NOT NULL
);

CREATE TABLE enseignant (
    id SERIAL PRIMARY KEY,
    personne_id INTEGER NOT NULL,
    matiere_id INTEGER NOT NULL,
    salaire DECIMAL(10, 2) NOT NULL,
    FOREIGN KEY (personne_id) REFERENCES personne(id),
    FOREIGN KEY (matiere_id) REFERENCES matiere(id)
);

CREATE TABLE etudiant (
    id SERIAL PRIMARY KEY,
    numCarte SERIAL UNIQUE,
    photoUrl VARCHAR(1000),
    personne_id INTEGER NOT NULL,
    FOREIGN KEY (personne_id) REFERENCES personne(id)
);

CREATE TABLE etudiant_cours (
    id SERIAL PRIMARY KEY,
    etudiant_id INTEGER NOT NULL,
    cours_id INTEGER NOT NULL,
    FOREIGN KEY (etudiant_id) REFERENCES etudiant(id),
    FOREIGN KEY (cours_id) REFERENCES cours(id)
);

CREATE TABLE formation (
    id SERIAL PRIMARY KEY,
    matiere_id INTEGER NOT NULL,
    nb_heures_semestre_1 INTEGER NOT NULL,
    nb_heures_semestre_2 INTEGER NOT NULL,
    nb_heures_semestres_3_et_4 INTEGER NOT NULL,
    FOREIGN KEY (matiere_id) REFERENCES matiere(id)
);

INSERT INTO ville (nom, codePostal) VALUES
('Paris', '75000'),
('Lyon', '69000'),
('Marseille', '13000'),
('Toulouse', '31000'),
('Nice', '06000'),
('Reims', '51100');

INSERT INTO personne (nom, prenom, sexe, isAdmin, ville_id, username, password, password_salt, needPasswordChange) VALUES
(
    'Admin',
    'Admin',
    'M',
    TRUE,
    (SELECT id FROM ville WHERE nom = 'Paris'),
    'admin',
    encode(digest('admin' || 'SALT_ADMIN_2026', 'sha256'), 'hex'),
    'SALT_ADMIN_2026',
    TRUE
),
(
    'Gougelet',
    'Estelle',
    'F',
    FALSE,
    (SELECT id FROM ville WHERE nom = 'Lyon'),
    'gougelet',
    encode(digest('gougelet' || 'SALT_GOUGELET_2026', 'sha256'), 'hex'),
    'SALT_GOUGELET_2026',
    TRUE
),
(
    'Lopes',
    'Miguel',
    'M',
    FALSE,
    (SELECT id FROM ville WHERE nom = 'Marseille'),
    'lopes',
    encode(digest('lopes' || 'SALT_LOPES_2026', 'sha256'), 'hex'),
    'SALT_LOPES_2026',
    TRUE
),
(
    'Dussart',
    'Justin',
    'M',
    FALSE,
    (SELECT id FROM ville WHERE nom = 'Toulouse'),
    'dussart',
    encode(digest('dussart' || 'SALT_DUSSART_2026', 'sha256'), 'hex'),
    'SALT_DUSSART_2026',
    TRUE
),
(
    'Pager',
    'Matteo',
    'M',
    FALSE,
    (SELECT id FROM ville WHERE nom = 'Nice'),
    'pager',
    encode(digest('pager' || 'SALT_PAGER_2026', 'sha256'), 'hex'),
    'SALT_PAGER_2026',
    TRUE
);

INSERT INTO matiere (nom, alias) VALUES
('Culture generale', 'CG'),
('Expression et communication en langue anglaise', 'ANGLAIS'),
('Mathematiques pour l''informatique', 'MATH-INFO'),
('Culture economique, juridique et manageriale', 'CEJM'),
('Culture economique, juridique et manageriale appliquee', 'CEJM-APP'),
('Support et mise a disposition de services informatiques', 'SUPPORT-SI'),
('SISR administration des systemes et des reseaux ou SLAM conception et developpement des reseaux', 'SISR/SLAM'),
('Cybersecurite des services informatiques', 'CYBER'),
('Ateliers de professionnalisation', 'AP'),
('Travail en autonomie en laboratoire', 'TAL'),
('Mathematiques approfondies', 'MATH-APP'),
('Parcours de certification complementaire', 'PCC');

INSERT INTO cours (nom) VALUES
('Culture generale'),
('Anglais'),
('Mathematiques pour l''informatique'),
('Culture economique, juridique et manageriale'),
('Culture economique, juridique et manageriale appliquee'),
('SISR'),
('SLAM'),
('Cyber'),
('Ateliers de professionnalisation'),
('Travail en autonomie en laboratoire'),
('Mathematiques approfondies'),
('Parcours de certification complementaire');

INSERT INTO formation (
    matiere_id,
    nb_heures_semestre_1,
    nb_heures_semestre_2,
    nb_heures_semestres_3_et_4
) VALUES
((SELECT id FROM matiere WHERE nom = 'Culture generale'), 3, 3, 2),
((SELECT id FROM matiere WHERE nom = 'Expression et communication en langue anglaise'), 3, 3, 2),
((SELECT id FROM matiere WHERE nom = 'Mathematiques pour l''informatique'), 3, 3, 3),
((SELECT id FROM matiere WHERE nom = 'Culture economique, juridique et manageriale'), 4, 4, 4),
((SELECT id FROM matiere WHERE nom = 'Culture economique, juridique et manageriale appliquee'), 1, 1, 1),
((SELECT id FROM matiere WHERE nom = 'Support et mise a disposition de services informatiques'), 10, 4, 2),
((SELECT id FROM matiere WHERE nom = 'SISR administration des systemes et des reseaux ou SLAM conception et developpement des reseaux'), 0, 6, 9),
((SELECT id FROM matiere WHERE nom = 'Cybersecurite des services informatiques'), 4, 4, 4),
((SELECT id FROM matiere WHERE nom = 'Ateliers de professionnalisation'), 4, 4, 4),
((SELECT id FROM matiere WHERE nom = 'Travail en autonomie en laboratoire'), 4, 4, 4),
((SELECT id FROM matiere WHERE nom = 'Mathematiques approfondies'), 2, 2, 2),
((SELECT id FROM matiere WHERE nom = 'Parcours de certification complementaire'), 2, 2, 2);

INSERT INTO enseignant (personne_id, matiere_id, salaire) VALUES
(
    (SELECT id FROM personne WHERE nom = 'Gougelet' AND prenom = 'Estelle'),
    (SELECT id FROM matiere WHERE nom = 'Culture generale'),
    3000.00
);

INSERT INTO etudiant (personne_id) VALUES
((SELECT id FROM personne WHERE nom = 'Lopes' AND prenom = 'Miguel')),
((SELECT id FROM personne WHERE nom = 'Dussart' AND prenom = 'Justin')),
((SELECT id FROM personne WHERE nom = 'Pager' AND prenom = 'Matteo'));

INSERT INTO etudiant_cours (etudiant_id, cours_id) VALUES
(
    (SELECT et.id FROM etudiant et JOIN personne p ON p.id = et.personne_id WHERE p.nom = 'Lopes' AND p.prenom = 'Miguel'),
    (SELECT id FROM cours WHERE nom = 'Culture generale')
),
(
    (SELECT et.id FROM etudiant et JOIN personne p ON p.id = et.personne_id WHERE p.nom = 'Dussart' AND p.prenom = 'Justin'),
    (SELECT id FROM cours WHERE nom = 'Culture generale')
),
(
    (SELECT et.id FROM etudiant et JOIN personne p ON p.id = et.personne_id WHERE p.nom = 'Pager' AND p.prenom = 'Matteo'),
    (SELECT id FROM cours WHERE nom = 'Culture generale')
);

CREATE VIEW getVilles AS
SELECT
    id,
    nom,
    codePostal
FROM ville;

CREATE VIEW getMatieres AS
SELECT
    id,
    nom,
    alias
FROM matiere;

CREATE VIEW getFormations AS
SELECT
    f.id AS formation_id,
    m.id AS matiere_id,
    m.nom AS matiere,
    m.alias,
    f.nb_heures_semestre_1,
    f.nb_heures_semestre_2,
    f.nb_heures_semestres_3_et_4
FROM formation f
JOIN matiere m ON f.matiere_id = m.id;

CREATE VIEW getEnseignants AS
SELECT
    e.id AS enseignant_id,
    p.id AS personne_id,
    p.nom,
    p.prenom,
    p.username,
    p.sexe,
    p.ville_id,
    v.nom AS ville,
    v.codePostal,
    e.matiere_id,
    m.nom AS matiere,
    m.alias,
    e.salaire
FROM enseignant e
JOIN personne p ON e.personne_id = p.id
JOIN matiere m ON e.matiere_id = m.id
LEFT JOIN ville v ON p.ville_id = v.id;

CREATE VIEW getEnseignantsDetails AS
SELECT
    e.id AS enseignant_id,
    p.id AS personne_id,
    p.nom,
    p.prenom,
    p.username,
    p.password,
    p.password_salt,
    p.needPasswordChange,
    p.sexe,
    p.isAdmin,
    p.ville_id,
    v.nom AS ville,
    v.codePostal,
    e.matiere_id,
    m.nom AS matiere,
    m.alias,
    e.salaire,
    'Enseignant' AS role
FROM enseignant e
JOIN personne p ON e.personne_id = p.id
JOIN matiere m ON e.matiere_id = m.id
LEFT JOIN ville v ON p.ville_id = v.id;

CREATE VIEW getEtudiants AS
SELECT
    et.id AS etudiant_id,
    et.numCarte,
    p.id AS personne_id,
    p.nom,
    p.prenom,
    p.username,
    p.sexe,
    p.ville_id,
    v.nom AS ville,
    v.codePostal,
    'Etudiant' AS role
FROM etudiant et
JOIN personne p ON et.personne_id = p.id
LEFT JOIN ville v ON p.ville_id = v.id;

CREATE VIEW getEtudiantsDetails AS
SELECT
    et.id AS etudiant_id,
    et.numCarte,
    p.id AS personne_id,
    p.nom,
    p.prenom,
    p.username,
    p.password,
    p.password_salt,
    p.needPasswordChange,
    p.sexe,
    p.isAdmin,
    p.ville_id,
    v.nom AS ville,
    v.codePostal,
    'Etudiant' AS role
FROM etudiant et
JOIN personne p ON et.personne_id = p.id
LEFT JOIN ville v ON p.ville_id = v.id;

CREATE VIEW getCours AS
SELECT
    ec.id AS etudiant_cours_id,
    et.id AS etudiant_id,
    et.numCarte,
    p.id AS personne_id,
    p.nom,
    p.prenom,
    c.id AS cours_id,
    c.nom AS cours
FROM etudiant_cours ec
JOIN etudiant et ON ec.etudiant_id = et.id
JOIN personne p ON et.personne_id = p.id
JOIN cours c ON ec.cours_id = c.id;

CREATE VIEW getComptesUtilisateurs AS
SELECT
    p.id,
    p.nom,
    p.prenom,
    p.username,
    p.password,
    p.password_salt,
    p.sexe,
    p.isAdmin,
    p.ville_id,
    p.needPasswordChange,
    v.nom AS ville,
    v.codePostal,
    et.id AS etudiant_id,
    et.numCarte,
    e.id AS enseignant_id,
    e.salaire,
    e.matiere_id,
    m.alias AS matiere,
    CASE
        WHEN p.isAdmin = TRUE THEN 'Admin'
        WHEN e.id IS NOT NULL THEN 'Enseignant'
        WHEN et.id IS NOT NULL THEN 'Etudiant'
        ELSE 'Utilisateur'
    END AS role
FROM personne p
LEFT JOIN ville v ON p.ville_id = v.id
LEFT JOIN etudiant et ON p.id = et.personne_id
LEFT JOIN enseignant e ON p.id = e.personne_id
LEFT JOIN matiere m ON e.matiere_id = m.id;

CREATE VIEW getLogin AS
SELECT *
FROM getComptesUtilisateurs;
