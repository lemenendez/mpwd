# My PassWorD management  ( mpwd)

A tool for maintain secure passwords. Secure account information like email and users. Database is protected with a paraphrase, the paragraph must be at least 30 characters long. Encrypts the storage. Done in modern C++, boost and CryptoPP.  Every password is identified by a key, the key must be unique.

## Usage

mpwd has a console interface: *mpwd.app*, the console app provides several commands: Init, Save, and Search.

### Init

The first step to securely store passwords is to initialize the data secure-storage.

#### Init Parameters

1. --p \<paraphrase\>
2. --f \<filename\>  (OPTIONAL)

#### Init Examples

Example 1: Initialize a secure database using a paraphrase without specifyng the filename

  ```shell
  ./mpwd.app --init --p InsecureParagrahaOnlyForDevTest12345$$
  ```

Example 2: Initialize a secure database specifyng a paraphrase and a file

```shell
mpwd.app --init --p Th1sAppIsAsS3cur3AsStrongIsYourParagr@ph -f /home/john/.secure-file-db
```

### Save

It add or update a account.

#### Save Parameters

1. --key \<unique_id\>
2. --val \<your_secret\>
3. --username \<your_user_name\>
4. --service \<service_name\>
5. --tags \<tag_list\>
6. --url \<the_url\>
7. --p \<paragraph\>
8. --f \<filename\>

#### Save Examples

Example 1: Stores (add if doesn't exist) the entry identified by *gmail* with the password *my_gmail_insecure_pass*

```shell
mpwd.app --p InsecureParagrahaOnlyForDevTest12345$$ --save --key gmail --val my_gmail_insecure_pass
```

Example 2: Stores (add if doesn't exist) the entry identified by *gmail* with the password *my_insecure_password* with the property username with *john.doe@gmail.com*

```shell
mpdw.app --p InsecureParagrahaOnlyForDevTest12345$$ --save --key gmail --val my_insecure_password --username john.doe@gmail.com
```

Example 3: Stores (add if doesn't exist) the entry identified by *gmail* with the password *my_insecure_password* with the property username with *john.doe@gmail.com* and the property tags with *email,personal*

```shell
mpdw.app --p InsecureParagrahaOnlyForDevTest12345$$ --save --key gmail --val my_insecure_password --username john.doe@gmail.com --tags email,personal
```

Example 4: Stores (add if doesn't exist) the entry identified by *gmail* with the password *my_insecure_password* with the property username with *john.doe@gmail.com* and the property *tags* with *email,personal* and the propery *url* with *https://gmail.com*

```shell
mpdw.app --p InsecureParagrahaOnlyForDevTest12345$$ --save --key gmail --val my_insecure_password --username john.doe@gmail.com --tags email,personal --url https://gmail.com
```

Check _dummy_db_base64.sh for more examples

### Search

It searchs into the datatabase and shows the actual password if the paramater *show* is especified, also shows the history of changes if the parameter *history* is specified.

#### Search Parameters

1. --keyword \<search_keywrod\>
2. --show
3. --history

#### Search Examples

Example 1: Searchs for the keyword gmail

```shell
mpwd.app --search --p InsecureParagrahaOnlyForDevTest12345$$ --keyword gmail
```

Example 2: Searches for the keyword gmail and shows the actuall password and history of changes

```shell
mpwd.app --search --p InsecureParagrahaOnlyForDevTest12345$$ --keyword gmail --show --history
```

Example 3: run the file _dummy_db.sh

```shell
./_dummy_db.sh
```

It creates a dummy database with debugging options: --encoding none --encrypt none
It creates 4 unique entries: JibJab, github, npm, and paypal.
You can open the file VERY_INSECURE_DB with the seralized objects.

## Debugging

The parameters *encrypt* and *encoding* canbe used independly to debug the mpwd
Options for encoding are: none (default), url (not supported), and base64
Options for encrypt are: none (debug) and salsa2

### Debugging Example

When working with the code use the *encrypt* parameter with the value *none*:

```shell
./bin/mpwd.app --init --f VERY_INSECURE_DB --p DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD --encoding none --encrypt none
```

The file content is:

```shell
22 serialization::archive 17 0 0 0  -374564288 89 DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD_DUMMY_PASSWORD 0 0 0 0
```

All the content is produced by the serialization system *boost*

Note: encoding and encrypt should be set when init, otherwise mpwd will throw an error.

## Testing

All the test are in the folder *tests*, to compile the test use the command:

```shell
make build-test
```

To run all the test use the command:

```shell
make test
```