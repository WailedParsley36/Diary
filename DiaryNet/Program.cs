using Newtonsoft.Json;

Console.OutputEncoding = System.Text.Encoding.UTF8;
Console.InputEncoding = System.Text.Encoding.UTF8;

LinkedList<DiaryRecord> records = FileManager.LoadRecords();
bool shouldRun = true;
while (shouldRun)
{
    Console.Clear();
    Console.WriteLine("Diary 2.0 -> .NET 6 Version");
    Console.WriteLine($"Loaded records: {records.Count}\n");

    Console.WriteLine("1. Show Records");
    Console.WriteLine("2. Create new Record at end");
    Console.WriteLine("3. Exit\n");

    switch (ReadUserInt(1, 3))
    {
        case 1:
            ShowRecords();
            break;

        case 2:
            records.AddLast(RunCreator());
            break;


        case 3:
            shouldRun = false;
            break;
    }
}

FileManager.SaveRecords(records);


void ShowRecords()
{
    bool showingRecords = true;

    int currentIndex = 0;

    LinkedListNode<DiaryRecord>? currentRecord = records.First;
    DiaryRecord temporaryRecord = null!;
    while (showingRecords)
    {
        Console.Clear();

        Console.WriteLine($"All records: {records.Count}");
        Console.WriteLine($"Current position: {currentIndex}\n");
        Console.WriteLine("1. Previous");
        Console.WriteLine("2. Next");
        Console.WriteLine("3. Delete");
        Console.WriteLine("4. Edit");
        Console.WriteLine("5. Insert new in this position");
        Console.WriteLine("6. Save current");
        Console.WriteLine("7. Back\n");

        if (currentRecord != null && temporaryRecord == null)
        {
            Console.WriteLine($"{currentRecord!.Value.Title} - {currentRecord.Value.DateRecorded}");
            Console.WriteLine($"\n{currentRecord.Value.Content}");
        }
        else if (temporaryRecord != null)
        {
            Console.WriteLine("!UNSAVED CHANGES!");

            Console.WriteLine($"{temporaryRecord!.Title} - {temporaryRecord.DateRecorded}");
            Console.WriteLine($"\n{temporaryRecord.Content}");
        }

        switch (ReadUserInt(1, 7))
        {
            case 1:
                if (currentRecord == null || currentRecord.Previous == null)
                    continue;

                currentIndex--;
                currentRecord = currentRecord.Previous;
                temporaryRecord = null!;
                break;

            case 2:
                if (currentRecord == null || currentRecord.Next == null)
                    continue;

                currentIndex++;
                currentRecord = currentRecord.Next;
                temporaryRecord = null!;
                break;

            case 3:
                if (currentRecord == null)
                    continue;

                records.Remove(currentRecord);

                currentIndex = 0;
                currentRecord = records.First;
                break;

            case 4:
                if (currentRecord == null)
                    continue;

                temporaryRecord = new DiaryRecord()
                {
                    Title = currentRecord.Value.Title,
                    Content = currentRecord.Value.Content,
                    DateRecorded = currentRecord.Value.DateRecorded
                };
                RunEditor(temporaryRecord);
                break;

            case 5:
                DiaryRecord newRecord = RunCreator();

                if (currentRecord == null)
                {
                    records.AddLast(newRecord);
                    currentRecord = records.First;
                }
                else
                {
                    records.AddAfter(currentRecord, newRecord);

                    currentIndex++;
                    currentRecord = currentRecord.Next;
                }
                break;

            case 6:
                if (currentRecord == null && temporaryRecord == null)
                    continue;

                currentRecord!.Value = temporaryRecord!;
                FileManager.SaveRecords(records);

                temporaryRecord = null!;
                break;

            case 7:
                showingRecords = false;
                FileManager.SaveRecords(records);
                break;
        }
    }
}


const string DateFormat = "dd.MM.yyyy HH:mm";

#region Methods
void RunEditor(DiaryRecord record)
{
    Console.Clear();
    Console.WriteLine($"Current date: {record.DateRecorded.ToString(DateFormat)}");
    Console.Write("Date: ");
    record.DateRecorded = ReadUserDateTime();

    Console.Clear();
    Console.WriteLine($"Current title: {record.Title}");
    Console.Write("Title: ");
    record.Title = Console.ReadLine()!;

    Console.Clear();
    Console.WriteLine($"Current content: {record.Content}");
    Console.WriteLine();
    record.Content = ReadMultipleUserLines();
}
DiaryRecord RunCreator()
{
    DiaryRecord newRecord = new DiaryRecord();

    Console.Clear();
    Console.Write("Date: ");
    newRecord.DateRecorded = ReadUserDateTime();

    Console.Clear();
    Console.Write("Title: ");
    newRecord.Title = Console.ReadLine()!;

    Console.Clear();
    Console.WriteLine("Diary Record Content");
    newRecord.Content = ReadMultipleUserLines();

    return newRecord;
}
#endregion

#region Utility
int ReadUserInt(int min, int max)
{
    int value = 0;
    while (!int.TryParse(Console.ReadLine(), out value) && (value < min || value > max))
        Console.Write($"Try again ({min}-{max}): ");

    return value;
}
DateTime ReadUserDateTime()
{
    Console.Write($"({DateFormat}): ");
    DateTime value = DateTime.Today;

    bool success = false;
    while (!success)
    {
        try
        {
            value = DateTime.ParseExact(Console.ReadLine()!, DateFormat, null);
            success = true;
        }
        catch
        {
            Console.Write($"Try again ({DateFormat}): ");
        }
    }

    return value;
}

string ReadMultipleUserLines()
{
    Console.WriteLine("Entered multiline input. To end simply write \"end\"");

    string result = string.Empty;

    string currentLine = "";
    while ((currentLine = Console.ReadLine()!) != "end")
        result += currentLine + '\n';

    return result;
}
#endregion

class DiaryRecord
{
    public string Title { get; set; }
    public string Content { get; set; }

    public DateTime DateRecorded { get; set; }
}

static class FileManager
{
    public static LinkedList<DiaryRecord> LoadRecords()
    {
        if (File.Exists(AppContext.BaseDirectory + "/diary.json"))
            return JsonConvert.DeserializeObject<LinkedList<DiaryRecord>>(File.ReadAllText(AppContext.BaseDirectory + "/diary.json"))!;
        else
            return new LinkedList<DiaryRecord>();
    }

    public static void SaveRecords(LinkedList<DiaryRecord> records)
    {
        File.WriteAllText(AppContext.BaseDirectory + "/diary.json", JsonConvert.SerializeObject(records));
    }
}
