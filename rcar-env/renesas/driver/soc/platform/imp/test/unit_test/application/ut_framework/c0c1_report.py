import sys, getopt
import re
import xlsxwriter
 
def create_report(report_file, result_file):
    print ("- Report File: " + report_file)
    print ("- Result File: " + result_file)
 
    # write to report
    workbook = xlsxwriter.Workbook(report_file)
 
    # format for header
    header_format = workbook.add_format({'font_size': 12})
    header_format.set_pattern(1) # solid fill
    header_format.set_bg_color('green')
    header_format.set_font_color('white')
 
    # failed r format
    failed_format = workbook.add_format()
    failed_format.set_pattern(1) # solid fill
    failed_format.set_bg_color('#FF4747')
    failed_format.set_num_format("@")
 
    # default format
    default_format = workbook.add_format()
    default_format.set_border()
 
    # summary sheet
    worksheet = workbook.add_worksheet("summary")
 
    # set column width
    worksheet.set_column(0, 0, 5)
    worksheet.set_column(1, 1, 38)
    worksheet.set_column(2, 2, 7)
    worksheet.set_column(3, 3, 7)
    worksheet.set_column(4, 4, 7)
 
    row = 0
    worksheet.write(row, 0, "No.", header_format)
    worksheet.write(row, 1, "Function", header_format)
    worksheet.write(row, 2, "C0", header_format)
    worksheet.write(row, 3, "C1", header_format)
    worksheet.write(row, 4, "MC/DC", header_format)
    row = row + 1
 
    # format coverage
    c0_format = r'"(\w+)"\s+statement coverage\s+(.+)%'
    c1_format = r'"(\w+)"\s+decision coverage\s+(.+)%'
    mcdc_format = r'"(\w+)"\s+multiple cond. coverage\s+(.+)%'
    func_name = ''
    cover_rate = [0, 0, 0]
    dt_report_info = {}
    func_count = 0
    aver_cover = [0, 0, 0]
 
    with open(result_file, "r") as file:
        for line in file:
            if re.search(c0_format, line) != None:
                func_name = re.search(c0_format, line).group(1)
                cover_rate[0] = float(re.search(c0_format, line).group(2))
                aver_cover[0] = aver_cover[0] + cover_rate[0]
 
            elif re.search(c1_format, line) != None:
                func_name = re.search(c1_format, line).group(1)
                cover_rate[1] = float(re.search(c1_format, line).group(2))
                aver_cover[1] = aver_cover[1] + cover_rate[1]
 
            elif re.search(mcdc_format, line) != None:
                func_name = re.search(mcdc_format, line).group(1)
                cover_rate[2] = float(re.search(mcdc_format, line).group(2))
                aver_cover[2] = aver_cover[2] + cover_rate[2]
 
            if func_name != '':
                if func_name not in dt_report_info:
                    dt_report_info[func_name] = [0, 0, 0]
 
                dt_report_info[func_name][0] = dt_report_info[func_name][0] + cover_rate[0]
                dt_report_info[func_name][1] = dt_report_info[func_name][1] + cover_rate[1]
                dt_report_info[func_name][2] = dt_report_info[func_name][2] + cover_rate[2]
                func_name = ''
                cover_rate = [0, 0, 0]
 
    for func in dt_report_info:
        func_count = func_count + 1
        worksheet.write(row, 0, func_count, default_format)
        worksheet.write(row, 1, func, default_format)
        worksheet.write(row, 2, dt_report_info[func][0], default_format)
        worksheet.write(row, 3, dt_report_info[func][1], default_format)
        worksheet.write(row, 4, dt_report_info[func][2], default_format)
        row = row + 1
   
    worksheet.write(row, 2, aver_cover[0] / func_count, default_format)
    worksheet.write(row, 3, aver_cover[1] / func_count, default_format)
    worksheet.write(row, 4, aver_cover[2] / func_count, default_format)
 
    # close workbook
    workbook.close()
 
def main(argv):
    result_file = ''
    report_file = ''
 
    try:
        opts, args = getopt.getopt(argv,"hi:",["result.ctr"])
    except getopt.GetoptError:
        print("c0c1_report.py -i <result.ctr>")
        sys.exit(2)
 
    for opt, arg in opts:
        if opt == '-h':
            print("c0c1_report.py -i <result.ctr>")
            sys.exit()
        elif opt in ("-i"):
            result_file = arg
 
    if result_file == '':
        print("missing result.ctr file\n")
        sys.exit()
 
    report_file = result_file.split("\\")[-1].split(".")[0] + ".xlsx"
    create_report(report_file, result_file)
 
if __name__ == "__main__":
   main(sys.argv[1:])